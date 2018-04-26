#ifndef FACTORY_H
#define FACTORY_H

#include <typeindex>
#include <string>
#include <map>
#include <list>
#include <mutex>
#include "log_hook.h"
#include "instancedeleter.h"
#include "explicit_ptr.h"

namespace goap
{

template<typename Interface, typename Base, typename Class>
void static_assert_internal()
{
    static_assert(std::is_base_of<Base, Interface>::value, "The Interface must derive from Base");
    static_assert(std::is_base_of<Interface, Class>::value, "The Class must derive from Interface");
}

template<typename T, typename ... Args>
T *defaultDelegate(Args ... args)
{
    return new T(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
T *singletonDelegate(Args ... args)
{
    return new T(std::forward<Args>(args)...);
}

/**
    @brief getClassName
    Helper template to retrieve a string representation of T.
    The string allocation happens only once.
    @return
*/
template<typename T>
const std::string &getClassName() noexcept
{
    static std::string ifName(typeid(T).name());
    return ifName;
}

template<typename T>
std::type_index getClassTypeIndex() noexcept
{
    return std::type_index(typeid(T));
}

enum class FactoryType
{
    Default,
    Singleton,
    //ObjectSmallPool,
    //ObjectMediumPool,
    //ObjectLargePool,
    //ThreadedDefault,
    //ThreadedSingleton,
    //ThreadedSmallPool,
    //ThreadedMediumPool,
    //ThreadedLargePool
};

namespace fact
{

/**
    @brief The factoryCreate struct. A helper to create new instances.
    A base class for partial specialisation.
*/
template<typename Base, typename Class, FactoryType fType>
struct factoryCreate
{
};

template<typename Base, typename Class>
struct factoryCreate<Base, Class, FactoryType::Default>
{
    typedef std::shared_ptr<Base> smart_pointer;
    inline static void dumbDeleter(Base *) {}

    template<typename F, typename C = Class, typename ... CallArgs>
    static typename std::enable_if<has_intrusive_ptr<C>::value, smart_pointer>::type
    getInstance(const F &func, CallArgs && ... args)
    {
        auto pInstance = func(std::forward<CallArgs>(args)...);
        C *p = dynamic_cast<C *>(pInstance);
        intrusive_ptr_add_ref(p);
        return smart_pointer(pInstance, [](Base * b)
        {
            C *p = dynamic_cast<C *>(b);
            intrusive_ptr_release(p);
        });
    }
    template<typename F, typename C = Class, typename ... CallArgs>
    static typename std::enable_if < !has_intrusive_ptr<C>::value, smart_pointer >::type
    getInstance(const F &func, CallArgs && ... args)
    {
        auto pInstance = func(std::forward<CallArgs>(args)...);
        return smart_pointer(pInstance, [](Base * b)
        {
            C *p = dynamic_cast<C *>(b);
            instanceSuicider(p);
        });
    }
};

template<typename Base, typename Class>
struct factoryCreate<Base, Class, FactoryType::Singleton>
{
    typedef std::shared_ptr<Base> smart_pointer;
public:
    template<typename F, typename ... CallArgs>
    static smart_pointer getInstance(const F &func,  CallArgs &&... args)
    {
        static smart_pointer singleton(func(std::forward<CallArgs>(args)...));
        return singleton;
    }
};

template<typename Base>
class BaseWrapperClass
{
public:
    typedef Base *return_type;
    virtual ~BaseWrapperClass() = default;
    virtual FactoryType getFactoryType() const = 0;
};


template<typename Base, typename ... Args>
class WrapperClass : public BaseWrapperClass<Base>
{
public:
    typedef std::function <typename BaseWrapperClass<Base>::return_type(Args...)> function_type;
    typedef std::shared_ptr<Base> smart_pointer;
    function_type func;

    template<typename F>
    WrapperClass(F &&f) : func(std::forward<F>(f)) {}
    virtual smart_pointer getInstance(Args &&... args) = 0;
};

template<typename Base, typename Class, FactoryType fType = FactoryType::Default, typename ... Args>
class WrapperClassTyped : public WrapperClass<Base, Args...>
{
public:
    typedef WrapperClass<Base, Args...> parent;
    typedef typename parent::smart_pointer smart_pointer;
    template<typename T>
    WrapperClassTyped(T &&func) : parent(std::forward<T>(func)) {}
    FactoryType getFactoryType() const override
    {
        return fType;
    }
    smart_pointer getInstance(Args &&... args) override
    {
        return factoryCreate<Base, Class, fType>::getInstance(parent::func, std::forward<Args>(args)...);
    }
};

}

using namespace fact;

template<typename Base, typename Key = std::string>
class Factory : public Base
{
public:
    //typedef explicit_ptr<Factory<Base, Key>> Ptr;
    Factory() {}
    virtual ~Factory() { clear(); }

    static Factory<Base, Key> &singleton()
    {
        static Factory<Base, Key> factory;
        return factory;
    }

    template<typename Interface = Base, typename ... Args>
    bool isInscribed(Key const &key = {}) const;

    template<typename Interface = Base, typename ... Args>
    explicit_ptr<Interface>
    create(Key const &key, Args && ... args) const;

    template<typename Interface = Base>
    explicit_ptr<Interface>
    create() const {
        return create<Interface>({});
    }

    template<typename Interface = Base, typename ... Args>
    std::map<Key, explicit_ptr<Interface>>
    createAll(Args && ... args) const;

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)> const &delegate,
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)> &&delegate,
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        Class * (*delegate)(Args ... args),
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class = Interface, typename ... Args>
    void inscribe(
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Lambda>
    void inscribe(
        Lambda const &lambda,
        Key const &key = Key());

    /**
     * @brief clear Clears the factory inscribed data
     */
    void clear();

private:
    template<FactoryType fType = FactoryType::Default, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
    void inscribe(
        ReturnType(Class::*)(Args...),
        Lambda const &lambda,
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
    void inscribe(
        ReturnType(Class::*)(Args...) const,
        Lambda const &lambda,
        Key const &key = Key());

    template<FactoryType fType, typename Interface, typename Class, typename ... Args>
    void inscribe(
        BaseWrapperClass<Base> *pWrapper,
        Key const &key = Key());

    template<typename Interface = Base, typename ... Args>
    WrapperClass<Base, Args...> *
    getWrapperClass(Key const &key) const;

protected:
    typedef Key key_type;
    typedef std::unique_ptr<BaseWrapperClass<Base>> value_type;
    mutable std::mutex _mutex;
    std::map<std::type_index, std::map<key_type, std::map<std::type_index, value_type>>> _map;
};


template<typename Base, typename Key>
template<typename Interface, typename ... Args>
WrapperClass<Base, Args...> *
Factory<Base, Key>::getWrapperClass(Key const &key) const
{
    std::type_index index = getClassTypeIndex<Interface>();
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _map.find(index);
    if (it == _map.end())
    {
        LOG(ERROR) << "Can't find interface \"" << index.name() << "\" in the factory.";
        return nullptr;
    }
    auto it2 = it->second.find(key);
    if (it2 == it->second.end())
    {
        LOG(ERROR) << "Can't find key " << key << " inside the interface \"" << index.name() << "\" in the factory.";
        return nullptr;
    }
    std::type_index subindex = getClassTypeIndex<WrapperClass<Base, Args...>>();
    auto it3 = it2->second.find(subindex);
    if (it3 == it2->second.end())
    {
        LOG(ERROR) << "Can't find registered constructor arguments in the factory.";
        return nullptr;
    }
    auto pWrapped = &*it3->second;
    typedef WrapperClass<Base, Args...> wrapper_t;
    wrapper_t *pWrapper = dynamic_cast<wrapper_t *>(pWrapped);
    if (!pWrapper)
    {
        LOG(ERROR) << "Can't cast from " << getClassName<decltype(*pWrapped)>() << " to " << getClassName<wrapper_t>();
    }
    return pWrapper;
}


template<typename Base, typename Key>
template<typename Interface, typename ...Args>
std::map<Key, explicit_ptr<Interface>>
Factory<Base, Key>::createAll(Args &&... args) const
{
    std::map<Key, explicit_ptr<Interface>> ret;
    std::type_index index = getClassTypeIndex<Interface>();
    std::list<Key> keys;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _map.find(index);
        if (it != _map.end())
        {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                keys.push_back(it2->first);
            }
        }
    }
    for (const auto &key : keys)
    {
        auto ptr = create<Interface, Args...>(key, std::forward<Args>(args)...);
        if (ptr)
        {
            ret[key] = ptr;
        }
    }
    return ret;
}

template<typename T>
inline void copyDefault(std::shared_ptr<T> &left, std::shared_ptr<T> &right)
{
    left = right;
}

#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
template<typename T>
inline void copyDefault(boost::intrusive_ptr<T> &left, std::shared_ptr<T> &right)
{
    T *ptr = right.get();
    left.reset(ptr);
}
#endif

template<typename T>
inline void copySingleton(std::shared_ptr<T> &left, std::shared_ptr<T> &right)
{
    left = right;
}

#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
template<typename T>
inline void copySingleton(boost::intrusive_ptr<T> &left, std::shared_ptr<T> &right)
{
    T *ptr = right.get();
    intrusive_ptr_add_ref(ptr);
    left.reset(ptr);
}
#endif

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
bool Factory<Base, Key>::isInscribed(Key const &key) const
{
    WrapperClass<Base, Args...> *pWrapper = getWrapperClass<Interface, Args...>(key);
    return pWrapper != nullptr;
}

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
explicit_ptr<Interface>
Factory<Base, Key>::create(Key const &key, Args &&... args) const
{
    explicit_ptr<Interface> ret;
    WrapperClass<Base, Args...> *pWrapper = getWrapperClass<Interface, Args...>(key);
    if (!pWrapper)
    {
        LOG(ERROR) << "Can't find a registered " << getClassName<Interface>() << " using the supplied arguments.";
        return ret;
    }
    auto smartInstance = pWrapper->getInstance(std::forward<Args>(args)...);
    auto ptr = std::dynamic_pointer_cast<Interface>(smartInstance);
    FactoryType factoryType = pWrapper->getFactoryType();
    if (factoryType == FactoryType::Default)
    {
        copyDefault(ret, ptr);
    }
    else if (factoryType == FactoryType::Singleton)
    {
        copySingleton(ret, ptr);
    }
    if (!ret)
    {
        LOG(ERROR) << "Can't cast from " << getClassName<decltype(*smartInstance)>() << " to " << getClassName<Interface>();
    }
    return ret;
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    BaseWrapperClass<Base> *pWrapper,
    Key const &key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    std::type_index subindex = getClassTypeIndex<WrapperClass<Base, Args...>>();
    std::lock_guard<std::mutex> lock(_mutex);
    _map[index][key][subindex] = value_type(pWrapper);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> const &delegate,
    Key const &key)
{
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...>(delegate);
    inscribe<fType, Interface, Class, Args...>(pWrapperClassTyped, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> &&delegate,
    Key const &key)
{
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...>(std::move(delegate));
    inscribe<fType, Interface, Class, Args...>(pWrapperClassTyped, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    Class * (*delegate)(Args ... args),
    Key const &key)
{
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...> (delegate);
    inscribe<fType, Interface, Class, Args...>(pWrapperClassTyped, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    Key const &key)
{
    inscribe<fType, Interface>(&defaultDelegate<Class, Args...>, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Lambda>
void Factory<Base, Key>::inscribe(
    Lambda const &lambda,
    Key const &key)
{
    inscribe<fType, Interface>(&Lambda::operator(), lambda, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
void Factory<Base, Key>::inscribe(
    ReturnType(Class::*)(Args...),
    Lambda const &lambda,
    Key const &key)
{
    std::function<ReturnType(Args...)> f = lambda;
    inscribe<fType, Interface>(f, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
void Factory<Base, Key>::inscribe(
    ReturnType(Class::*)(Args...) const,
    Lambda const &lambda,
    Key const &key)
{
    std::function<ReturnType(Args...)> f = lambda;
    inscribe<fType, Interface>(f, key);
}

template<typename Base, typename Key>
void Factory<Base, Key>::clear()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _map.clear();
}


}

#endif // FACTORY_H
