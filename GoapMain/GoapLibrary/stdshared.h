#ifndef PTRSHARED_H
#define PTRSHARED_H

#include <functional>
#include <atomic>
#include "irefcounter.h"
#include "privatefactory.h"


// http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence


/**
 * Check for the existence of a member callable with the `member_call` signature
 * returning a type converitible to `member_type`.
 * The `member_call` works even for pure virtual function invocations.
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

// http://zchydem.enume.net/2010/01/19/qt-howto-private-classes-and-d-pointers/

/*
 * Instead of using the Qt Q_DECLARE_PRIVATE macros family, here is pressented
 * an alternative system with the addition of allowing to derive the
 * class private data for reusing by the corresponding derived public classes.
 */
template <typename T> static inline T *
dGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::pointer
dGetPtrHelper(const Wrapper &p) { return const_cast<typename Wrapper::pointer>(p.data()); }

#define PTR_DECLARE_PRIVATE(Class) \
protected: \
    inline Class##Private* d_func() { \
      return reinterpret_cast<Class##Private *>(dGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { \
      return reinterpret_cast<const Class##Private *>(dGetPtrHelper(d_ptr)); } \
    friend class Class##Private;


#define PTR_SHARED_QFUNCTIONS(refc) \
    virtual void suicide() { ::ByTech::PrivateFactory::instanceDeleter(this, 0); } \
    inline int load() const { return this->refc.load(); } \
    inline void store(int count) { this->refc.store(count); } \
    inline bool addRef() const { return this->refc.ref(); } \
    inline bool releaseRef() const { return this->refc.deref(); }

/** @brief PTR_DECLARE_SHARED macro
 *  Declaration for implementing an IRefCounter-like interface (but not requiring it!!!).
 *  It also includes the suicide() function that is usefull for safe deletion:
 *  If you delete an object from one interface, you must provide a valid
 *  destructor for that interface, so no pure virtual classes can be used.
 *  With the suicide() function that is not necessary because it allways use
 *  the pointer "this" to delete the real objects instead a pointer to a interface.
 */
#define PTR_DECLARE_SHARED  \
public: \
    mutable std::atomic ref; \
    PTR_SHARED_QFUNCTIONS(ref)

// This macro is the dummy version of PTR_DECLARE_SHARED.
// It disables Ptr deletion possibility (a explicit delete is needed).
#define PTR_DECLARE_DUMMY_SHARED \
public: \
    virtual void suicide() { } \
    inline int load() const { return -1; } \
    inline void store(int) {  } \
    inline bool addRef() const { return -1; } \
    inline bool releaseRef() const { return -1; }

/**
 * @brief The PtrSharedData class
 * A base class implementing the IRefCounter interface.
 */
class PtrSharedData : public IRefCounter
{
    PTR_DECLARE_SHARED
public:
    inline PtrSharedData() { }
    inline virtual ~PtrSharedData() = default;
    inline PtrSharedData(const PtrSharedData &) { }

private:
    // using the assignment operator would lead to corruption in the ref-counting
    PtrSharedData &operator=(const PtrSharedData &);
};

template <typename T>
class WrapWithSharedData : public T, virtual public IRefCounter
{
    PTR_DECLARE_SHARED
public:
    inline virtual ~WrapWithSharedData() = default;
};

template <typename T>
class WrapQSharedData : public T, virtual public IRefCounter
{
public:
    PTR_SHARED_QFUNCTIONS(ref)
    inline virtual ~WrapQSharedData() = default;
};

HAS_MEMBER(bool, releaseRef(), bool__releaseRef);
HAS_MEMBER(bool, addRef(), bool__addRef);
HAS_MEMBER(int, load(), int__load);
HAS_MEMBER(void, suicide(), void__suicide);
HAS_MEMBER(void*, clone(), voidp__clone);
HAS_MEMBER(QAtomicInt, ref, QAtomicInt__ref);

template <typename T>
class has_by_ref_counter
{
public:
    static bool const value =
        has_member_void__suicide<T>::value
        && has_member_bool__releaseRef<T>::value
        && has_member_bool__addRef<T>::value
        && has_member_int__load<T>::value;
};


/**
 * @brief The PtrDeleter class
 * Helper class for object deletion.
 * When the static function deleter() is called then:
 * - If T implements the funcion T::suicide() then it will be called.
 * - Otherways a normal delete is used.
 */
template<class T>
class PtrDeleter
{
public:
    /// Allows the instance to be self deleted
    static void deleter(T*t) { PrivateFactory::instanceSuicider(t, 0); }
};

template<class T>
class PtrNoDeleter
{
public:
    static void deleter(T*) { } // Never deletes the instance
};

/**
 * @brief Ptr Smart pointer for explicit shared data with custom deleter.
 * Similar to QExplicitlySharedDataPointer but
 * using a IRefCounter interface.
 *
 * If you need a object clone, do call detach().
 * The T type must implement the load(), addRef(), and releaseRef() functions.
 * The D type must implement a static deleter().
 */
template <class T, class D = PtrDeleter<T>>
class Ptr
{
public:
    typedef T Type;
    typedef T *pointer;
    typedef D Deleter;

    static void deleter(T* t) { D::deleter(t);}

    inline T &operator*() const { return *d; }
    inline T *operator->() { return d; }
    inline T *operator->() const { return d; }
    inline T *data() const { return d; }
    inline const T *constData() const { return d; }

    inline void detach() { if (d && d->load() != 1) detach_helper(); }

    inline void reset()
    {
        if(d && !d->releaseRef())
            Ptr::deleter(d);

        d = nullptr;
    }

    inline operator bool () const { return d != nullptr; }

    inline bool operator==(const Ptr<T,D> &other) const { return d == other.d; }
    inline bool operator!=(const Ptr<T,D> &other) const { return d != other.d; }
    inline bool operator==(const T *ptr) const { return d == ptr; }
    inline bool operator!=(const T *ptr) const { return d != ptr; }

    inline Ptr() { d = nullptr; }
    inline ~Ptr() { if (d && !d->releaseRef()) Ptr<T,D>::deleter(d); }

    inline Ptr(T *data) noexcept;
    inline Ptr(const Ptr<T,D> &o) : d(o.d) { if (d) d->addRef(); }

    template<class X>
    inline Ptr(const Ptr<X,D> &o)
        : d(o.data())
    {
        if(d)
            d->addRef();
    }

    inline Ptr<T,D> & operator=(const Ptr<T,D> &o) {
        if (o.d != d) {
            if (o.d)
                o.d->addRef();
            T *old = d;
            d = o.d;
            if (old && !old->releaseRef())
                Ptr::deleter(old);
        }
        return *this;
    }
    inline Ptr &operator=(T *o) {
        if (o != d) {
            if (o)
                o->addRef();
            T *old = d;
            d = o;
            if (old && !old->releaseRef())
                Ptr::deleter(old);
        }
        return *this;
    }

    inline Ptr(Ptr &&o) noexcept : d(o.d) { o.d = nullptr; }
    inline Ptr<T,D> &operator=(Ptr<T,D> &&other) noexcept
    { qSwap(d, other.d); return *this; }

    inline bool operator!() const { return !d; }

    inline void swap(Ptr &other) noexcept
    { qSwap(d, other.d); }

protected:
    /**
     * @brief clone
     * When implementing a copy on write pattern using interfaces,
     * it is needed a way for clone the real object. If the `clone()` function
     * is provided, it will be used over a explicit `new` memory reservation.
     */
    template <class Ti=T>
    typename std::enable_if<has_member_voidp__clone<Ti>::value, Ti*>::type
    clone()
    {
        return dynamic_cast<Ti*>(d->clone());
    }

    /**
     * @brief clone
     * Default clone to be used when no `clone()` function is available.
     * That is the case of the constuctor copiable classes.
     */
    template <class Ti=T>
    typename std::enable_if<!has_member_voidp__clone<Ti>::value, Ti*>::type
    clone()
    {
        return new T(*d);
    }

protected:
    void detach_helper();

    T *d;
};

/**
 * @brief Cow Smart pointer for explicit shared data with custom deleter.
 * Similar to QSharedDataPointer but
 * using a IRefCounter interface.
 *
 * If you need a object clone, do call detach().
 * However detach() is automatically called when the internal data is to be
 * modified. This is a implementation of the Copy On Write (COW) pattern.
 * The T type must implement the load(), addRef(), and releaseRef() functions.
 * The D type must implement a static deleter().
 */
template <class T, class D = PtrDeleter<T>>
class Cow : public Ptr<T,D>
{
public:
    inline T &operator*() { Ptr<T,D>::detach(); return *this->d; }
    inline const T &operator*() const { return *this->d; }
    inline T *operator->() { Ptr<T,D>::detach(); return this->d; }
    inline const T *operator->() const { return this->d; }
    inline operator T *() { Ptr<T,D>::detach(); return this->d; }
    inline operator const T *() const { return this->d; }
    inline T *data() { Ptr<T,D>::detach(); return this->d; }
    inline const T *data() const { return this->d; }
    inline const T *constData() const { return this->d; }

    inline bool operator==(const Cow<T,D> &other) const { return this->d == other.d; }
    inline bool operator!=(const Cow<T,D> &other) const { return this->d != other.d; }

    inline Cow() { this->d = nullptr; }
    inline ~Cow() { if (this->d && !this->d->releaseRef()) Ptr<T,D>::deleter(this->d); }

    inline Cow(T *data) noexcept : Ptr<T,D>(data) {}
    inline Cow(const Cow<T,D> &o) : Ptr<T,D>(o.d) { }

    template<class X>
    inline Cow(const Cow<X,D> &o) : Ptr<T,D>(o.d) { }

    inline Cow<T,D> & operator=(const Cow<T,D> &o) {
        if (o.d != this->d) {
            if (o.d)
                o.d->addRef();
            T *old = this->d;
            this->d = o.d;
            if (old && !old->releaseRef())
                Ptr<T,D>::deleter(this->d);
        }
        return *this;
    }

    inline Cow(Cow &&o) : Ptr<T,D>(o.d) { o.d = nullptr; }
    inline Cow<T,D> &operator=(Cow<T,D> &&other) noexcept
    { qSwap(this->d, other.d); return *this; }


    inline bool operator!() const { return !this->d; }

    inline void swap(Cow &other) noexcept
    { qSwap(this->d, other.d); }
};


template <class T, class D>
void Ptr<T,D>::detach_helper()
{
    T *x = clone();
    x->addRef();
    if (!d->releaseRef())
        Ptr::deleter(d);
    d = x;
}

template <class T, class D>
Ptr<T,D>::Ptr(T *adata) noexcept
    : d(adata)
{ if (d) d->addRef(); }

template <class T, class D>
void qSwap(Ptr<T,D> &p1, Ptr<T,D> &p2)
{ p1.swap(p2); }


template <class T, class D>
uint qHash(
        const ByTech::Ptr<T,D> &ptr, uint seed = 0) noexcept
{
    return qHash(ptr.data(), seed);
}

#endif // PTRSHARED_H
