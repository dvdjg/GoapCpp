#ifndef FACTORY_H
#define FACTORY_H

#include <typeindex>
#include <utility>
#include <memory>
#include <string>
#include <map>

namespace goap {

template<typename Interface, typename Base, typename PtrType>
void static_assert_internal()
{
    static_assert( std::is_pointer<PtrType>::value, "The return type must be a pointer of type Base*");
    static_assert( std::is_base_of<Base, typename std::remove_pointer<PtrType>::type>::value,
                   "Callable needs to return Base*");
    static_assert( std::is_base_of<Interface, typename std::remove_pointer<PtrType>::type>::value, "Type must derive from Interface");
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
    Factory() {}
    typedef std::unique_ptr<Base> return_type;

    template<typename Interface = Base, typename ... Args>
    std::unique_ptr<Interface>
    Create(
        Key const& key,
        Args && ... args);

    /// registers lvalue std::functions
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        std::function<Return (Args ... args)> const& delegate,
        Key const& key = Key());

    /// registers rvalue std::functions
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        std::function<Return (Args ... args)>&& delegate,
        Key const& key = Key());

    /// registers function pointer
    template<typename Interface = Base, typename Return, typename ... Args>
    void Register(
        Return (*delegate) (Args ... args),
        Key const& key = Key());

//	/// registers zero-argument lambdas like []() { return ...; }
//	template<typename Lambda>
//	void Register(
//		Key const& key,
//		Lambda const& lambda);

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
Factory<Base, Key>::Create(
    Key const& key,
    Args&& ... args)
{
    std::type_index index = getClassTypeIndex<Interface>();
    auto ret = _map.find(index);
    if (ret == _map.end())
        return nullptr;
    auto ret2 = ret->second.find(key);
    if (ret2 == ret->second.end())
        return nullptr;
    typedef WrapperClass<Base, Args...> wrapper_t;
    try
    {
        //auto& pType = ret2->second;
        //wrapper_t* pwrapper = dynamic_cast<wrapper_t*>(pType);
        //auto ptr = (*pwrapper)(std::forward<Args>(args)...);
        //auto const& rType = *pType;
        //wrapper_t const& wrapper = dynamic_cast<wrapper_t const&>(*(ret2->second));
        //auto ptr = wrapper(std::forward<Args>(args)...);
        //auto pint = dynamic_cast<Interface*>(ptr);
        //auto ret = return_type(ptr);
        auto p = &*ret2->second;
        auto* pder = dynamic_cast<wrapper_t const*>(p);
        auto pint = (*pder)(std::forward<Args>(args)...);
        auto pret = dynamic_cast<Interface*>(pint);
        return std::unique_ptr<Interface>(pret);
    }
    catch (std::bad_cast&)
    {
        return nullptr;
    }
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    std::function<Return (Args ... args)> const& delegate,
    Key const& key)
{
    static_assert_internal<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...>(delegate));
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    std::function<Return (Args ... args)>&& delegate,
    Key const& key)
{
    static_assert_internal<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...>(std::move(delegate)));
}

template<typename Base, typename Key>
template<typename Interface, typename Return, typename ... Args>
void Factory<Base, Key>::Register(
    Return (*delegate) (Args ... args),
    Key const& key)
{
    static_assert_internal<Interface, Base, Return>();
    std::type_index index = getClassTypeIndex<Interface>();
    _map[index][key] = value_type(new WrapperClass<Base, Args...> (delegate));
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
