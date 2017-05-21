#ifndef FACTORY_H
#define FACTORY_H

#include <typeindex>
#include <utility>
#include <memory>
#include <string>
#include <map>

namespace goap {

template<typename Interface, typename Base, typename TYPE>
void ASSERT_INTERNAL_TYPES()
{
    static_assert( std::is_pointer<TYPE>::value, "The return type must be a pointer of type Base*");
    static_assert( std::is_base_of<Base, typename std::remove_pointer<TYPE>::type>::value,
                   "Callable needs to return Base*");
    static_assert( std::is_base_of<Interface, Base>::value, "Base must derive from Interface");
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

protected:
    function_type _func;
};


template<typename Base, typename Key = std::string>
class Factory
{
public:
    typedef std::unique_ptr<Base> return_type;

    template<typename Interface = Base, typename ... Args>
    std::unique_ptr<Interface>
    Create(
        Key const& key,
        Args && ... args);

    /// registers lvalue std::functions
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        Key const& key,
        std::function<Return (Args ... args)> const& delegate);

    /// registers rvalue std::functions
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        Key const& key,
        std::function<Return (Args ... args)>&& delegate);

    /// registers function pointer
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        Key const& key,
        Return (*delegate) (Args ... args));

//	/// registers zero-argument lambdas like []() { return ...; }
//	template<typename Lambda>
//	void Register(
//		Key const& key,
//		Lambda const& lambda);

protected:
    typedef Key key_type;
    typedef std::unique_ptr<BaseWrapperClass<Base>> value_type;
    std::map<std::type_index, std::map<key_type, value_type>> _map;
};


// implementation

template<typename Base, typename Key>
template<typename Interface, typename ... Args>
typename std::unique_ptr<Interface>// Factory<Base, Key>::return_type
Factory<Base, Key>::Create(
    Key const& key,
    Args&& ... args)
{
    std::type_index index = getClassTypeIndex<Interface>();
    auto ret = _map.find(index);
    if (ret == _map.end())
        return nullptr;
    auto ret2 = ret.second.find(key);
    if (ret2 == ret.second.end())
        return nullptr;
    typedef WrapperClass<Base, Args...> wrapper_t;
    try
    {
        wrapper_t const& wrapper = dynamic_cast<wrapper_t const&>(*(ret2.second));
        return std::unique_ptr<Interface>(wrapper(std::forward<Args>(args)...));
    }
    catch (std::bad_cast&)
    {
        return nullptr;
    }
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    Key const& key,
    std::function<Return (Args ... args)> const& delegate)
{
    ASSERT_INTERNAL_TYPES<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = return_type(new WrapperClass<Base, Args...>(delegate));
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    Key const& key,
    std::function<Return (Args ... args)>&& delegate)
{
    ASSERT_INTERNAL_TYPES<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = return_type(new WrapperClass<Base, Args...>(std::move(delegate)));
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    Key const& key,
    Return (*delegate) (Args ... args))
{
    ASSERT_INTERNAL_TYPES<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = return_type(new WrapperClass<Base, Args...> (delegate));
}

//template<typename Key = std::string>
//class Factory
//{
//public:
//    typedef std::unique_ptr<Base> return_type;
////    template<typename Interface, typename Class, FactoryType = FactoryType::Default>
////    bool registerClass()
////    {
////    }
//private:
//    //std::map<std::type_index, std::map<std::string, >>
//};
}

#endif // FACTORY_H
