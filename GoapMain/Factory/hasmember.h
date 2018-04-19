#ifndef HASMEMBER_H
#define HASMEMBER_H

#include "utility_simple.h"

namespace goap
{

#if defined(BOOST_SMART_PTR_INTRUSIVE_PTR_HPP_INCLUDED)
#if !defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
#define HAS_BOOST_SMART_INTRUSIVE_PTR
#endif
#endif

/** @brief HAS_MEMBER(member_type, member_call, templ_postfix)
    Check for the existence of a member callable with the `member_call` signature
    returning a type converitible to `member_type`.
    The `member_call` works even for pure virtual function invocations.
    @sa http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
*/
#define HAS_MEMBER(member_type, member_call, templ_postfix) \
    template <typename T> \
    class has_member_##templ_postfix \
    { \
        typedef char one; \
        typedef long two; \
        \
        template <typename C> static two test(...); \
        template <typename C> static one test(typename enable_if<is_convertible<decltype(static_cast<C*>(nullptr)->member_call), member_type>::value>::type *); \
    public: \
        enum { evalue = sizeof(test<T>(0)) == sizeof(char) }; \
        static bool const value = sizeof(test<T>(0)) == sizeof(char); \
    }

#define HAS_GLOBAL(member_type, global_call, templ_postfix) \
    template <typename T> \
    class has_global_##templ_postfix \
    { \
        typedef char one; \
        typedef long two; \
        \
        template <typename C> static two test(...); \
        template <typename C> static one test(typename enable_if<is_convertible<decltype(global_call(static_cast<C*>(nullptr))), member_type>::value>::type *); \
    public: \
        enum { evalue = sizeof(test<T>(0)) == sizeof(char) }; \
        static bool const value = sizeof(test<T>(0)) == sizeof(char); \
    }

// Helper testers to determine if a class implements a functionality.
HAS_MEMBER(int, releaseRef(), int__releaseRef);
HAS_MEMBER(int, addRef(), int__addRef);
HAS_MEMBER(int, loadRef(), int__load);
HAS_MEMBER(void, suicide(), void__suicide);
HAS_MEMBER(void *, clone(), voidp__clone);
HAS_MEMBER(void *, get(), voidp__get);
HAS_MEMBER(void *, data(), voidp__data);
HAS_MEMBER(void, reset(), void__reset);
HAS_MEMBER(void, clear(), void__clear);
HAS_MEMBER(const void *, constData(), voidp__constData);
HAS_MEMBER(void, deleteLater(), void__deleteLater);
//HAS_MEMBER(QAtomicInt, ref, QAtomicInt__ref);

HAS_GLOBAL(void, intrusive_ptr_add_ref, void__intrusive_ptr_add_ref);
HAS_GLOBAL(void, intrusive_ptr_release, void__intrusive_ptr_release);

/**
    @brief The has_ref_counter<T> class
    The internal 'value' will be true if T implements the needed functions
    for a effective reference counting through function members.
*/
template <typename T>
class has_ref_counter
{
public:
    static bool const value =
        has_member_int__releaseRef<T>::value
        && has_member_int__addRef<T>::value
        && has_member_int__load<T>::value;
};


/**
    @brief The has_intrusive_ptr<T> class
    Test if the type T can be used with the boost intrusive_ptr_add_ref() and intrusive_ptr_release() functions
*/
template <typename T>
class has_intrusive_ptr
{
public:
    static bool const value = has_global_void__intrusive_ptr_add_ref<T>::value && has_global_void__intrusive_ptr_release<T>::value;
};

/**
    @brief The has_recyclable_ref_counter<T> class
    If the class implements the suicide() member function, and match
    has_goap_ref_counter, then it is fine for the recycling mechanism of object pools.
*/
template <typename T>
class has_recyclable_ref_counter
{
public:
    static bool const value = has_member_void__suicide<T>::value && has_ref_counter<T>::value;
};

}

#endif // HASMEMBER_H
