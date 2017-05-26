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
struct factory_cast
{
    typedef typename std::conditional<has_intrusive_ref_counter<T>::value, boost::intrusive_ptr<T>, std::shared_ptr<T>>::type smart_pointer;
    static smart_pointer cast(T *p, FactoryType factoryType = FactoryType::Default)
    {
        if (factoryType == FactoryType::Singleton)
        {
            static smart_pointer singleton(p);
            return singleton;
        }
        else if (factoryType == FactoryType::Default)
        {
            return p;
        }
        return nullptr;
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

    template<typename T>
    WrapperClass(T &&func) : _func(std::forward<T>(func)) {}

    typename function_type::result_type
    operator()(Args &&... args) const
    {
        return _func(std::forward<Args>(args)...);
    }
    //    template<typename ... CallArgs>
    //    typename function_type::result_type
    //    call(CallArgs&& ... args) const
    //    {
    //        return _func(std::forward<CallArgs>(args)...);
    //    }

protected:
    function_type _func;
};

template<typename Base, FactoryType fType = FactoryType::Default, typename ... Args>
class WrapperClassTyped : public WrapperClass<Base, Args...>
{
public:
    template<typename T>
    WrapperClassTyped(T &&func) : WrapperClass<Base, Args...>(func) {}
    FactoryType getFactoryType() const override
    {
        return fType;
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

    template<typename Interface = Base, typename ... Args>
    Interface *
    createRaw(
        Key const &key,
        Args && ... args);

    template<typename Interface = Base, typename ... Args>
    WrapperClass<Base, Args...> *
    getWrapperClass(Key const &key);

    template<typename Interface = Base, typename ... Args>
    typename factory_cast<Interface>::smart_pointer
    create(Key const &key, Args && ... args);

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)> const &delegate,
        Key const &key = Key());

    template<FactoryType fType = FactoryType::Default, typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)> && delegate,
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

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
Interface *Factory<Base, Key>::createRaw(
    Key const &key,
    Args &&... args)
{
    auto pWrapper = getWrapperClass<Interface, Args...>(key);
    if (!pWrapper)
    {
        return nullptr;
    }
    auto pObj = (*pWrapper)(std::forward<Args>(args)...);
    auto pInterface = dynamic_cast<Interface *>(pObj);
    if (!pInterface && pObj)
    {
        std::cerr << "Can't cast from " << getClassName<decltype(*pObj)>() << " to " << getClassName<Interface>();
        instanceSuicider(pObj);
    }
    return pInterface;
}

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
typename factory_cast<Interface>::smart_pointer
Factory<Base, Key>::create(Key const &key, Args &&... args)
{
    auto pWrapper = getWrapperClass<Interface, Args...>(key);
    if (!pWrapper)
    {
        return nullptr;
    }
    auto pInterface = dynamic_cast<Interface *>((*pWrapper)(std::forward<Args>(args)...));
    return factory_cast<Interface>::cast(pInterface, pWrapper->getFactoryType());
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> const &delegate,
    Key const &key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    auto pWrapperClassTyped = new WrapperClassTyped<Base, fType, Args...>(delegate);
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
    auto pWrapperClassTyped = new WrapperClassTyped<Base, fType, Args...>(std::move(delegate));
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
    auto pWrapperClassTyped = new WrapperClassTyped<Base, fType, Args...> (delegate);
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
template<FactoryType fType, typename Interface, typename Class, typename R, typename Lambda, typename... Args>
void Factory<Base, Key>::inscribe(
    R(Class::*)(Args...),
    Lambda const &lambda,
    Key const &key)
{
    std::function<R(Args...)> f = lambda;
    inscribe<fType, Interface>(f, key);
}

template<typename Base, typename Key>
template<FactoryType fType, typename Interface, typename Class, typename R, typename Lambda, typename... Args>
void Factory<Base, Key>::inscribe(
    R(Class::*)(Args...) const,
    Lambda const &lambda,
    Key const &key)
{
    std::function<R(Args...)> f = lambda;
    inscribe<fType, Interface>(f, key);
}

}

#endif // FACTORY_H
