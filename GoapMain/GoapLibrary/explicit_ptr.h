#ifndef EXPLICIT_PTR_H
#define EXPLICIT_PTR_H

#include <boost/intrusive_ptr.hpp>
#include "hasmember.h"

namespace goap
{
template <typename T> using explicit_ptr = typename std::conditional<has_intrusive_ptr<T>::value, boost::intrusive_ptr<T>, std::shared_ptr<T>>::type;
}

#endif
