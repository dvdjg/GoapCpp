#ifndef EXPLICIT_PTR_H
#define EXPLICIT_PTR_H

#include <memory>
#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
#include <boost/intrusive_ptr.hpp>
#include "hasmember.h"
#endif

namespace goap
{
#if defined(HAS_BOOST_SMART_INTRUSIVE_PTR)
template <typename T> using explicit_ptr = typename std::conditional<has_intrusive_ptr<T>::value, boost::intrusive_ptr<T>, std::shared_ptr<T>>::type;
#else
template <typename T> using explicit_ptr = std::shared_ptr<T>;
#endif

}

#endif
