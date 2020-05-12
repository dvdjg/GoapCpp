#ifndef EXPLICIT_PTR_H
#define EXPLICIT_PTR_H

#ifndef HAS_BOOST_SMART_INTRUSIVE_PTR
#define HAS_BOOST_SMART_INTRUSIVE_PTR
#endif

#include <memory>
#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
#include <boost/intrusive_ptr.hpp>
#include "hasmember.h"
#else
#include <shared_ptr.h>
#endif

namespace goap
{
#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
template <typename T> using explicit_ptr = typename std::conditional<has_intrusive_ptr<T>::value, boost::intrusive_ptr<T>, std::shared_ptr<T>>::type;

//template <class T, class U>
//inline boost::intrusive_ptr<T> dynamic_ptr_cast (const boost::intrusive_ptr<U>& sp) noexcept {
//    return boost::intrusive_ptr<T>(dynamic_cast<T*>(sp.get()));
//}

#else
template <typename T> using explicit_ptr = std::shared_ptr<T>;
#endif

//template <class T, class U>
//inline std::shared_ptr<T> dynamic_ptr_cast (const std::shared_ptr<U>& sp) noexcept {
//    return std::dynamic_pointer_cast<T>(sp);
//}

}

#endif
