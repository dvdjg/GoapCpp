#ifndef FACTORY_H
#define FACTORY_H

#include <typeindex>
#include <utility>
#include <memory>
#include <string>
#include <map>
#include <iostream>

namespace goap {

template<typename Interface, typename Base, typename Class>
void static_assert_internal()
{
    static_assert( std::is_base_of<Base, Interface>::value, "The Interface must derive from Base");
    static_assert( std::is_base_of<Interface, Class>::value, "The Class must derive from Interface");
}

template<typename T, typename ... Args>
T* delegate(Args ... args)
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
const std::string& getClassName() noexcept
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
    ObjectSmallPool,
    ObjectMediumPool,
    ObjectLargePool,
    ThreadedDefault,
    ThreadedSingleton,
    ThreadedSmallPool,
    ThreadedMediumPool,
    ThreadedLargePool
};

template<typename Base>
class BaseWrapperClass
{
public:
    typedef Base* return_type;
    virtual ~BaseWrapperClass() {}
};


template<typename Base, typename ... Args>
class WrapperClass : public BaseWrapperClass<Base>
{
public:
    typedef std::function <
    typename BaseWrapperClass<Base>::return_type (Args...)
    > function_type;

    template<typename T>
    WrapperClass(T&& func) : _func(std::forward<T>(func)) {}

    typename function_type::result_type
    operator() (Args&& ... args) const
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


template<typename Base, typename Key = std::string>
class Factory
{
public:
    Factory() {}
    typedef std::unique_ptr<Base> return_type;

    template<typename Interface = Base, typename ... Args>
    Interface *
    createRaw(
        Key const& key,
        Args && ... args);

    template<typename Interface = Base, typename ... Args>
    std::unique_ptr<Interface>
    create(
        Key const& key,
        Args && ... args);

    /// registers lvalue std::functions
    template<typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)> const& delegate,
        Key const& key = Key());

    /// registers rvalue std::functions
    template<typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        std::function<Class* (Args ... args)>&& delegate,
        Key const& key = Key());

    /// registers function pointer
    template<typename Interface = Base, typename Class, typename ... Args>
    void inscribe(
        Class * (*delegate) (Args ... args),
        Key const& key = Key());

    /// registers class
//    template<typename Interface = Base, typename Class, typename ... Args>
//    void inscribe(
//        Class* (*delegate) (Args ... args),
//        Key const& key = Key());
    static Factory<Base, Key>& singleton()
    {
        static Factory<Base, Key> factory;
        return factory;
    }

protected:
    typedef Key key_type;
    typedef std::unique_ptr<BaseWrapperClass<Base>> value_type;
    std::map<std::type_index, std::map<key_type, value_type>> _map;
};


// implementation

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
typename std::unique_ptr<Interface>// Factory<Base, Key>::return_type
Factory<Base, Key>::create(
    Key const& key,
    Args&& ... args)
{
    std::type_index index = getClassTypeIndex<Interface>();
    auto it = _map.find(index);
    if (it == _map.end())
        return nullptr;
    auto ret2 = it->second.find(key);
    if (ret2 == it->second.end())
        return nullptr;
    typedef WrapperClass<Base, Args...> wrapper_t;
    try
    {
        wrapper_t const& wrapper = dynamic_cast<wrapper_t const&>(*(ret2->second));
        auto pint = dynamic_cast<Interface*>(wrapper(std::forward<Args>(args)...));
        return std::unique_ptr<Interface>(pint);
    }
    catch (std::bad_cast& e)
    {
        std::cerr << "Bad cast " << e.what();
        return nullptr;
    }
}

template<typename Base, typename Key>
template<typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)> const& delegate,
    Key const& key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...>(delegate));
}

template<typename Base, typename Key>
template<typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    std::function<Class* (Args ... args)>&& delegate,
    Key const& key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...>(std::move(delegate)));
}

template<typename Base, typename Key>
template<typename Interface, typename Class, typename ... Args>
void Factory<Base, Key>::inscribe(
    Class * (*delegate) (Args ... args),
    Key const& key)
{
    static_assert_internal<Interface, Base, Class>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...> (delegate));
}

}

#endif // FACTORY_H
