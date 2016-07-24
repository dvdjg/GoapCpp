/**
    @brief Forward declarations for the Ptr, Cow and HideFactory template classes
    @file
*/

#ifndef PTR_FORWARD_H
#define PTR_FORWARD_H

namespace goap
{
class IFactoryItem;

/** @brief The HideFactory<> template class.

    Forward declaration of a template class meant to hide smart pointer details.
    The common use of this class is for creating typedefs storing a smart pointer
    type.

        typedef HideFactory<IMyData> PtrIMyData;

    Use:
        PtrIMyData data; // You don't see smart pointer factory creation

        data->setData("Hello");

    You can force the inner smart pointer type of your choice:

        typedef HideFactory<IMyData, boost::shared_ptr<IMyData>> PtrIMyData;

    Use:
        PtrIMyData data; // The implementation allways uses the same typedef.

        data->setData("Hello");

    @sa factory.h
*/
template<typename I, typename V = I, typename B = IFactoryItem> class HideFactory;
template<class T> class PtrDeleter;
template <class T, class D = PtrDeleter<T>> class Ptr;
template <class T, class D = PtrDeleter<T>> class Cow;

}

#endif // PTR_FORWARD_H
