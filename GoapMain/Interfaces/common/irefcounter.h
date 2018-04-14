#ifndef IREFCOUNTER_H
#define IREFCOUNTER_H

#include <utility>
#include "iroot.h"


namespace goap
{

/**
    @brief The IRefCounter class
    The object implementing this interface has an atomic reference counter.
    This is useful for using SmartPointers or ObjectPools.
*/
class IRefCounter : public IRoot
{
protected:
    /**
        @brief loadRef
        The returned value is only usefull when there is no other threads
        using concurrently the object reference counter.
        @return Internal counter value.
    */
    virtual int loadRef() const = 0;

    /**
        @brief ref Increments internal reference counter.
    */
    virtual int addRef() const = 0;

    /**
        @brief deref Atomically decrements the value of this counter.
    */
    virtual int releaseRef() const = 0;

    /**
        @brief suicide
        External petition for deletion.
        The implementation should do something like: "delete this"
    */
    virtual void suicide() = 0;

    friend void intrusive_ptr_add_ref(const IRefCounter *t);
    friend void intrusive_ptr_release(IRefCounter *t);
};



//namespace details {
//template<typename... Ts> struct make_void { typedef void type;};
//template<typename... Ts> using void_t = typename make_void<Ts...>::type;

//    template <typename Base> std::true_type is_base_of_test_func(const volatile Base*);
//    template <typename Base> std::false_type is_base_of_test_func(const volatile void*);
//    template <typename Base, typename Derived>
//    using pre_is_base_of = decltype(is_base_of_test_func<Base>(std::declval<Derived*>()));

//    // with <experimental/type_traits>:
//    // template <typename Base, typename Derived>
//    // using pre_is_base_of2 = std::experimental::detected_or_t<std::true_type, pre_is_base_of, Base, Derived>;
//    template <typename Base, typename Derived, typename = void>
//    struct pre_is_base_of2 : public std::true_type { };
//    // note std::void_t is a C++17 feature
//    template <typename Base, typename Derived>
//    struct pre_is_base_of2<Base, Derived, void_t<pre_is_base_of<Base, Derived>>> :
//        public pre_is_base_of<Base, Derived> { };
//}

//template <typename Base, typename Derived>
//struct is_base_of :
//    public std::conditional_t<
//        std::is_class<Base>::value && std::is_class<Derived>::value,
//        details::pre_is_base_of2<Base, Derived>,
//        std::false_type
//    > { };

//template<class B, class D>
//struct is_base_of
//{
//  template<typename T> struct dummy {};
//  template<typename T = D>
//  struct Child : T{};
//  static B* Check (const volatile B*);
//  static char Check(const volatile void*);

//  static const bool value = (sizeof(Check(static_cast<Child<D> *>(0))) == sizeof(B*));
//};

using namespace std;

template< class From, class To > using isconvertible = std::is_convertible< From, To >;

template<typename T>
inline typename std::enable_if <isconvertible<T*, IRefCounter*>::value, void>::type
intrusive_ptr_add_ref(const T *t)
{
    intrusive_ptr_add_ref(static_cast<const IRefCounter*>(t));
}

//template<>
inline void intrusive_ptr_add_ref(const IRefCounter *t)
{
    t->addRef();
}

template<typename T>
inline typename std::enable_if <isconvertible<T*, IRefCounter*>::value, void>::type
intrusive_ptr_release(T *t)
{
    intrusive_ptr_release(static_cast<IRefCounter*>(t));
}

inline void intrusive_ptr_release(IRefCounter *t)
{
    if (t->releaseRef() == 0)
    {
        t->suicide();
    }
}
}

//#define IMPLEMENT_INTRUSIVE_EX(T, B) \
//    template <> inline void intrusive_ptr_add_ref(const T *t) { intrusive_ptr_add_ref(static_cast<const B*>(t)); } \
//    template <> inline void intrusive_ptr_release(T *t) { intrusive_ptr_release(static_cast<B*>(t)); }

//#define IMPLEMENT_INTRUSIVE(T) IMPLEMENT_INTRUSIVE_EX(T, IRefCounter)

#endif // IREFCOUNTER_H
