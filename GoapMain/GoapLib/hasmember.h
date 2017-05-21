#ifndef HASMEMBER_H
#define HASMEMBER_H

#include <functional>

namespace goap {

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
        template <typename C> static one test( typename std::enable_if< \
                                               std::is_convertible<decltype( \
                                                       static_cast<C*>(nullptr)->member_call), member_type>::value>::type * \
                                             ); \
    public: \
        enum { evalue = sizeof(test<T>(0)) == sizeof(char) }; \
        static bool const value = sizeof(test<T>(0)) == sizeof(char); \
    }

// Helper testers to determine if a class implements a functionality.
HAS_MEMBER(bool, releaseRef(), bool__releaseRef);
HAS_MEMBER(bool, addRef(), bool__addRef);
HAS_MEMBER(int, load(), int__load);
HAS_MEMBER(void, suicide(), void__suicide);
HAS_MEMBER(void*, clone(), voidp__clone);
HAS_MEMBER(void*, get(), voidp__get);
HAS_MEMBER(void*, data(), voidp__data);
HAS_MEMBER(void, reset(), void__reset);
HAS_MEMBER(const void*, constData(), voidp__constData);
//HAS_MEMBER(QAtomicInt, ref, QAtomicInt__ref);
HAS_MEMBER(void, deleteLater(), void__deleteLater);


}

#endif // HASMEMBER_H
