#ifndef FACTORY_H
#define FACTORY_H

#include <typeindex>
#include <utility>
#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <mutex>
#include "refcounter.h"

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
    Undefined,
    Default,
    Singleton,
    ObjectSmallPool,
    ObjectMediumPool,
    ObjectLargePool,
    ThreadedDefault,
    ThreadedSingleton,
    ThreadedSmallPool,
    ThreadedMediumPool,
    ThreadedLargePool
};


template<typename T>
struct factoryCast
{
    typedef typename std::conditional<has_intrusive_ref_counter<T>::value, boost::intrusive_ptr<T>, std::shared_ptr<T>>::type smart_pointer;
    //    static smart_pointer cast(T *p, FactoryType factoryType = FactoryType::Default)
    //    {
    //        if (factoryType == FactoryType::Singleton)
    //        {
    //            static smart_pointer singleton(p);
    //            return singleton;
    //        }
    //        else if (factoryType == FactoryType::Default)
    //        {
    //            return p;
    //        }
    //        return nullptr;
    //    }
};

template<typename Base, typename Class, FactoryType fType>
struct factoryCreate
{
    typedef std::shared_ptr<Base> smart_pointer;
};

template<typename Base, typename Class>
struct factoryCreate<Base, Class, FactoryType::Default>
{
    typedef std::shared_ptr<Base> smart_pointer;
    inline static void dumbDeleter(Base *) {}
    template<typename F, typename ... CallArgs>
    static smart_pointer getInstance(F &func, CallArgs &&... args)
    {
        auto pInstance = func(std::forward<CallArgs>(args)...);
        return smart_pointer(pInstance, dumbDeleter);
    }
};

template<typename Base, typename Class>
struct factoryCreate<Base, Class, FactoryType::Singleton>
{
    typedef std::shared_ptr<Base> smart_pointer;
public:
    template<typename F, typename ... CallArgs>
    static smart_pointer getInstance(F &func,  CallArgs &&... args)
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
    typedef std::shared_ptr<Base> base_smart_pointer;

    template<typename T>
    WrapperClass(T &&func) : _func(std::forward<T>(func)) {}

    typename function_type::result_type
    operator()(Args &&... args) const
    {
        return _func(std::forward<Args>(args)...);
    }
    template<typename ... CallArgs>
    typename function_type::result_type
    call(CallArgs &&... args) const
    {
        return _func(std::forward<CallArgs>(args)...);
    }
    function_type &getFunc() const
    {
        return _func;
    }
    virtual base_smart_pointer getInstance(Args &&... args) = 0;

protected:
    function_type _func;
};

template<typename Base, typename Class, FactoryType fType = FactoryType::Default, typename ... Args>
class WrapperClassTyped : public WrapperClass<Base, Args...>
{
public:
    typedef WrapperClass<Base, Args...> parent;
    typedef typename parent::base_smart_pointer base_smart_pointer;
    template<typename T>
    WrapperClassTyped(T &&func) : parent(std::forward<T>(func)) {}
    FactoryType getFactoryType() const override
    {
        return fType;
    }
    base_smart_pointer getInstance(Args &&... args) override
    {
        auto instance = factoryCreate<Base, Class, fType>::getInstance(parent::_func, std::forward<Args>(args)...);
        return instance;
    }
};

template<typename Base, typename Key = std::string>
class Factory : public Base
{
    IMPLEMENT_REFCOUNTER()

public:
    Factory() {}

    static Factory<Base, Key> &singleton()
    {
        static Factory<Base, Key> factory;
        return factory;
    }

    //template<typename Interface = Base, typename ... Args>
    //Interface *
    //createRaw(
    //    Key const &key,
    //    Args && ... args);

    template<typename Interface = Base, typename ... Args>
    WrapperClass<Base, Args...> *
    getWrapperClass(Key const &key);

    template<typename Interface = Base, typename ... Args>
    typename factoryCast<Interface>::smart_pointer
    create(Key const &key, Args && ... args);

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
        Key const &key);

private:
    template<FactoryType fType = FactoryType::Default, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
    void inscribe(
        ReturnType(Class::*)(Args...),
        Lambda const &lambda,
        Key const &key);

    template<FactoryType fType = FactoryType::Default, typename Interface, typename Class, typename ReturnType, typename Lambda, typename... Args>
    void inscribe(
        ReturnType(Class::*)(Args...) const,
        Lambda const &lambda,
        Key const &key);

protected:
    typedef Key key_type;
    typedef std::unique_ptr<BaseWrapperClass<Base>> value_type;
    std::mutex _mutex;
    std::map<std::type_index, std::map<key_type, value_type>> _map;
};


template<typename Base, typename Key>
template<typename Interface, typename ... Args>
WrapperClass<Base, Args...> *
Factory<Base, Key>::getWrapperClass(Key const &key)
{
    std::type_index index = getClassTypeIndex<Interface>();
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _map.find(index);
    if (it == _map.end())
    {
        std::cerr << "Can't find interface \"" << index.name() << "\" in the factory.";
        return nullptr;
    }
    auto it2 = it->second.find(key);
    if (it2 == it->second.end())
    {
        std::cerr << "Can't find key " << key << " inside the interface \"" << index.name() << "\" in the factory.";
        return nullptr;
    }
    auto pWrapped = &*it2->second;
    typedef WrapperClass<Base, Args...> wrapper_t;
    wrapper_t *pWrapper = dynamic_cast<wrapper_t *>(pWrapped);
    if (!pWrapper)
    {
        std::cerr << "Can't cast from " << getClassName<decltype(*pWrapped)>() << " to " << getClassName<wrapper_t>();
    }
    return pWrapper;
}

//template<typename Base, typename Key>
//template<typename Interface, typename ... Args>
//Interface *Factory<Base, Key>::createRaw(
//    Key const &key,
//    Args &&... args)
//{
//    auto pWrapper = getWrapperClass<Interface, Args...>(key);
//    if (!pWrapper)
//    {
//        return nullptr;
//    }
//    auto pObj = (*pWrapper)(std::forward<Args>(args)...);
//    auto pInterface = dynamic_cast<Interface *>(pObj);
//    if (!pInterface && pObj)
//    {
//        std::cerr << "Can't cast from " << getClassName<decltype(*pObj)>() << " to " << getClassName<Interface>();
//        instanceSuicider(pObj);
//    }
//    return pInterface;
//}

template<typename T>
inline void copySingleton(std::shared_ptr<T> &left, std::shared_ptr<T> &right)
{
    left = right;
}

template<typename T>
inline void copySingleton(boost::intrusive_ptr<T> &left, std::shared_ptr<T> &right)
{
    T *ptr = right.get();
    intrusive_ptr_add_ref(ptr);
    left.reset(ptr);
}

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
typename factoryCast<Interface>::smart_pointer
Factory<Base, Key>::create(Key const &key, Args &&... args)
{
    typename factoryCast<Interface>::smart_pointer ret;
    WrapperClass<Base, Args...> *pWrapper = getWrapperClass<Interface, Args...>(key);
    if (!pWrapper)
    {
        std::cerr << "Can't finmd a registered class " << getClassName<Interface>() << " using the supplied arguments.";
        return ret;
    }
    //https://stackoverflow.com/questions/12340810/using-custom-deleter-with-stdshared-ptr
    auto smartInstance = pWrapper->getInstance(std::forward<Args>(args)...);
    FactoryType factoryType = pWrapper->getFactoryType();
    if (factoryType == FactoryType::Default)
    {
        auto pInterface = dynamic_cast<Interface *>(smartInstance.get()); // smartInstance must have a dumb deleter
        ret.reset(pInterface);
    }
    else if (factoryType == FactoryType::Singleton)
    {
        auto ptr = std::dynamic_pointer_cast<Interface>(smartInstance);
        copySingleton(ret, ptr);
    }
    return ret;
    //    auto pObj = (*pWrapper)(std::forward<Args>(args)...);
    //    auto pInterface = dynamic_cast<Interface *>(pObj);
    //    if (!pInterface && pObj)
    //    {
    //        std::cerr << "Can't cast from " << getClassName<decltype(*pObj)>() << " to " << getClassName<Interface>();
    //        instanceSuicider(pObj);
    //    }
    //    return factoryCast<Interface>::cast(pInterface, pWrapper->getFactoryType());
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> const &delegate,
    Key const &key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...>(delegate);
    std::lock_guard<std::mutex> lock(_mutex);
    _map[index][key] = value_type(pWrapperClassTyped);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> &&delegate,
    Key const &key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...>(std::move(delegate));
    std::lock_guard<std::mutex> lock(_mutex);
    _map[index][key] = value_type(pWrapperClassTyped);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    Class * (*delegate)(Args ... args),
    Key const &key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    auto pWrapperClassTyped = new WrapperClassTyped<Base, Class, fType, Args...> (delegate);
    std::lock_guard<std::mutex> lock(_mutex);
    _map[index][key] = value_type(pWrapperClassTyped);
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

}

#endif // FACTORY_H
