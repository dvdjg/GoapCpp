#ifndef UTILITY_SIMPLE_H
#define UTILITY_SIMPLE_H

namespace goap
{

template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> { typedef T type; };

template<class B, class D>
struct is_convertible
{
  template<typename T> struct dummy {};
  template<typename T = D>
  struct Child : T{};
  static B* Check (const volatile B*);
  static char Check(const volatile void*);

  static const bool value = (sizeof(Check(static_cast<Child<D> *>(0))) == sizeof(B*));
};

template<class B>
struct is_convertible<B, B>
{
    static const bool value = true;
};

}

#endif // UTILITY_SIMPLE_H
