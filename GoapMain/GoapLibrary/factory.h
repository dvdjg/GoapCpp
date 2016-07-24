/** @brief Class factory pattern implementation.

    The Factory depends on the Smart pointers system and the ObjectPool .

    @file factory.h
*/

#ifndef GOAP_FACTORY_H
#define GOAP_FACTORY_H

#include <QMap>
#include <QString>
#include <QtDebug>
#include <QReadWriteLock>
#include <QException>
#include "shared.h"
#include "objectpool.h"

namespace goap
{

#ifndef GOAP_COMMA
#define GOAP_COMMA ,
#endif

/**
    @brief The FactoryType enum
    Different registration policies for object instantation.
*/
enum class FactoryType
{
    INVALID,
    DEFAULT,    // The object will be created by ordinary methods (eg. new)
    SINGLETON,  // The object will be a singleton
    SMALL_POOL, // The object creation will use a quarter of a default pool (128)
    POOL,       // The object creation will use a default pool size (512)
    BIG_POOL    // The object creation will use four times a default pool (2048)
};

/************************************************************************
    Factory Policy
*/
template<typename T>
class ICreationPolicy
{
public:
    virtual ~ICreationPolicy() = default;
    virtual FactoryType factoryType() const = 0;
    virtual T *create() const = 0;
    virtual void recycle(T *ptr) const = 0;
};

namespace PrivateFactory
{

// Discrimination using C++11 "trailing return type syntax" ->
/**
    @brief singletonCreator
    This function will be used to create instances using the
    T::singleton() static function if available.
    @return The address of the single object.
*/
template <typename T>
inline auto singletonCreator(int, int, int) -> decltype((&T::singleton)())
{
    return T::singleton();
}

template <typename T>
class RefCounterClassCreator
{
public:
    T t;
    RefCounterClassCreator()
    {
        t.addRef();
    }
    ~RefCounterClassCreator()
    {
    }
};

/**
    @brief singletonCreator
    This function will be used to create singletons
    if T has a reference counting system using the functions:
    addRef(), releaseRef() and load().
    @return The address of the single object.
*/
template <typename T>
inline typename std::enable_if <has_goap_ref_counter<T>::value, T *>::type
singletonCreator(long, int, int)
{
    // Maintain the object with at least one reference
    //    static Ptr<T> object(new T);
    //    return object.data();
    static RefCounterClassCreator<T> object;
    return &object.t;
}

/**
    @brief singletonCreator
    This function will be used to create singletons
    if T has a reference counting system using a member
    object `ref` implementing the functions:
    ref.addRef(), ref.releaseRef() and ref.load().
    @return The address of the single object.
*/
template <typename T>
inline typename std::enable_if < !has_goap_ref_counter<T>::value
&&has_member_QAtomicInt__ref<T>::value, T * >::type
singletonCreator(long, long, int)
{
    // Maintain the object with at least one reference
    static QSharedDataPointer<T> object(new T);
    return object.data();
}

/**
    @brief singletonCreator
    This function will be used to create singletons as static variables.
    @return The address of the single object.
*/
template <typename T>
inline typename std::enable_if < !has_goap_ref_counter<T>::value
&&!has_member_QAtomicInt__ref<T>::value, T * >::type
singletonCreator(long, long, long)
{
    // In C++11, this is thread safe. According to the standard, §6.7 p4:
    // * If control enters the declaration concurrently while the variable
    //   is being initialized, the concurrent execution shall wait for
    //   completion of the initialization.
    static T object;
    return &object;
}

template<typename T, typename TDerived>
class PolicyInstance : public ICreationPolicy<T>
{
public:
    FactoryType factoryType() const override
    {
        return FactoryType::DEFAULT;
    }
    T *create() const override
    {
        return instanceCreator<TDerived>(0);
    }
    void recycle(T *) const override {}
};

template<typename T, typename TDerived>
class PolicySingleton : public ICreationPolicy<T>
{
public:
    FactoryType factoryType() const override
    {
        return FactoryType::SINGLETON;
    }
    T *create() const override
    {
        return goap::PrivateFactory::singletonCreator<TDerived>(0, 0, 0);
    }
    void recycle(T *) const override {}
};

template<typename T, typename TDerived, size_t Size = DEFAULT_POOL_SIZE>
class PolicyPool : public ICreationPolicy<T>
{
public:
    FactoryType factoryType() const override
    {
        return (Size < DEFAULT_POOL_SIZE) ? FactoryType::SMALL_POOL :
               (Size > DEFAULT_POOL_SIZE) ? FactoryType::BIG_POOL :
               FactoryType::POOL;
    }
    T *create() const override
    {
        auto pool = ObjectPool<TDerived, Size>::singleton();
        return pool->createRaw();
    }
    /**
        @brief recycle
        Recycles the object. If it can't be recycled then it is deleted
        @param ptr
    */
    void recycle(T *ptr) const override
    {
        auto pool = ObjectPool<TDerived>::singleton();
        auto obj = dynamic_cast<TDerived *>(ptr);
        bool bRecycled = pool->recycle(obj);
        if (!bRecycled)
        {
            instanceDeleter(obj, 0);
        }
    }
};

template<typename T>
class PolicyNull : public ICreationPolicy<T>
{
public:
    PolicyNull() {}
    FactoryType factoryType() const override
    {
        return FactoryType::INVALID;
    }
    T *create() const override
    {
        return nullptr;
    }
    void recycle(T *) const override {}
};

}

/**
    @brief The IFactory class interface
    Interface for implementing the internal storage core of any factory class.
*/
template <typename T>
class IFactory : virtual public IRefCounter
{
public:

    /**
        @brief setCreationFunction
        Binds the name of an interface with the pointer of a function
        that is able to return an object implementing it.
        @param interfaceName Interface name.
        @param opName Operation name.
        @param pCreateFunc Address of a function for creating an object
        implementing the T interface.
        @return true if the binding happens. False if a previous interface
        name was already registerd with the same name.
    */
    virtual bool setCreationFunction(
        const QString &interfaceName,
        const QString &opName,
        const ICreationPolicy<T> *pCreateFunc) = 0;

    /**
        @brief getCreationPolicy
        Returns the function pointer stored with setCreationFunction().
        @param interfaceName The interface name.
        @param opName Operation name.
        @return The function pointer or nullptr if no unique object is stored
        with the associated interfaceName for the supplied opName.
    */
    virtual const ICreationPolicy<T> *getCreationPolicy(
        const QString &interfaceName,
        const QString &opName = QString()) const = 0;

    /**
        @brief getOperationsForInterface
        Returns a list of registered operation names for a given interface name.
        @param interfaceName
        @return
    */
    virtual QStringList getOperationsForInterface(
        const QString &interfaceName) const = 0;
};

/**
    @brief The FactoryPrivate class
    Implememnts the IFactory<T> interface to be the basis of the private
    data for a Factory class.
*/
template <typename T>
class FactoryPrivate : virtual public IFactory<T>
{
    GOAP_DECLARE_SHARED
public:
    //typedef typename IFactory<T>::FactoryCreateFunc FactoryCreateFunc;

    /**
        @brief _createFuncs
        Interface Name -> Discriminator Name -> Creation function
    */
    QMap<QString, QMap<QString, const ICreationPolicy<T>*>> _createFuncs;
    mutable QReadWriteLock _lock;

    bool setCreationFunction(const QString &interfaceName,
                             const QString &opName,
                             const ICreationPolicy<T> *pCreateFunc) override
    {
        bool ret = false;
        QWriteLocker locker(&_lock);
        if (_createFuncs.contains(interfaceName)
            && _createFuncs[interfaceName].contains(opName))
        {
            qWarning() << "Factory::registerTypeName Type " << interfaceName
                       << " for op " << opName << " already registered.";
        }
        else
        {
            _createFuncs[interfaceName][opName] = pCreateFunc;
            ret = true;
        }
        return ret;
    }

    const ICreationPolicy<T> *getCreationPolicy(
        const QString &interfaceName,
        const QString &opName) const override
    {
        static const PrivateFactory::PolicyNull<T> policyNull;
        const ICreationPolicy<T> *ret = &policyNull;
        QReadLocker locker(&_lock);
        auto itOps = _createFuncs.find(interfaceName);
        if (itOps != _createFuncs.end())
        {
            auto itCr = itOps.value().find(opName);
            if (itCr != itOps.value().end())
            {
                ret = itCr.value();
            }
            else if (opName.isEmpty())
            {
                qWarning() << "Factory::create Type" << interfaceName
                           << "needs a explicit registered operation name.";
            }
            else
            {
                qWarning() << "Factory::create Type" << interfaceName
                           << "for op" << opName << "not registered.";
            }
        }
        else
        {
            qWarning() << "Factory::create Type" << interfaceName
                       << "not registered.";
        }
        return ret;
    }

    QStringList getOperationsForInterface(
        const QString &interfaceName) const override
    {
        QStringList ret;
        QReadLocker locker(&_lock);
        auto itOps = _createFuncs.find(interfaceName);
        if (itOps != _createFuncs.end())
        {
            ret = itOps.value().keys();
        }
        return ret;
    }
};

/**
    @brief The Factory<T,P> class template.

    This class template implements the class factory pattern for
    class creation. It also implements a system to bind
    classes with their main interfaces so the class instances can
    be created when asking for a interface implementor.

    The `T` type is the base class of all types that can be registered/created
    with this factory.

    The `P` type is the base class for the Factory itself. It is `T` by default
    but can be any other type (eg. IFactoryItem).
*/
template <typename T, typename P = T>
class Factory : virtual public P
{
protected:


    template <typename TDerived>
    bool registerByTypeName(const QString &interfaceName,
                            const QString &opName = QString(),
                            FactoryType factoryType = FactoryType::DEFAULT)
    {
        bool bRet = false;
        const auto isBase = ::std::is_base_of<T GOAP_COMMA TDerived>::value;
        static_assert(isBase,
                          "Can't register class because it does not "
                          "derive from the factory base.");

        const ICreationPolicy<T> *pCreateFunc = nullptr;
        switch (factoryType)
        {
            case FactoryType::SINGLETON:
                {
                    static PrivateFactory::PolicySingleton<T, TDerived> policy;
                    pCreateFunc = &policy;
                    break;
                }
            case FactoryType::SMALL_POOL:
                {
                    static PrivateFactory::PolicyPool < T, TDerived, DEFAULT_POOL_SIZE / 4 > policy;
                    pCreateFunc = &policy;
                    break;
                }
            case FactoryType::POOL:
                {
                    static PrivateFactory::PolicyPool<T, TDerived, DEFAULT_POOL_SIZE> policy;
                    pCreateFunc = &policy;
                    break;
                }
            case FactoryType::BIG_POOL:
                {
                    static PrivateFactory::PolicyPool<T, TDerived, DEFAULT_POOL_SIZE * 4> policy;
                    pCreateFunc = &policy;
                    break;
                }
            case FactoryType::DEFAULT:
            case FactoryType::INVALID:
            default:
                {
                    static PrivateFactory::PolicyInstance<T, TDerived> policy;
                    pCreateFunc = &policy;
                    break;
                }
        }
        bRet = d->setCreationFunction(interfaceName,
                                      opName,
                                      pCreateFunc);
        return bRet;
    }

    const ICreationPolicy<T> *policyByInterfaceName(
        QString interfaceName,
        const QString &opName = QString()) const
    {
        return d->getCreationPolicy(interfaceName, opName);
    }

public:
    /**
        @brief Factory
        Default constructor
    */
    Factory() : d(new FactoryPrivate<T>)
    {
        // Self registration as a singleton
        registerType<Factory<T, P>>(FactoryType::SINGLETON);
    }

    /**
        @brief Factory
        Creates a factory referencing other factory.
        @param privateData
    */
    Factory(IFactory<T> *privateData) :
        d((privateData == nullptr) ? new FactoryPrivate<T> : privateData) {}

    /**
        @brief Factory
        Creates a factory referencing other factory.
        @param rhs
    */
    Factory(const Factory &rhs) : d(rhs.d) {}

    /**
        @brief init
        Sets the private data of the factory.
        @param privateData
    */
    void init(IFactory<T> *privateData)
    {
        if (d != privateData)
        {
            d = privateData;
        }
    }

    /**
        @brief operator =
        Assign operator.
        @param rhs
        @return
    */
    Factory<T, P> &operator=(const Factory<T, P> &rhs)
    {
        if (this != &rhs)
        {
            d.operator = (rhs.d);
        }
        return *this;
    }

    /**
        @brief operator ==
        Equality operator.
        @param rhs
        @return
    */
    bool operator==(const Factory<T, P> &rhs)
    {
        return d.operator == (rhs.d);
    }

    /**
        @brief data
        Returns a pointer to the internal storage object.
        This is usefull for passing a reference of this factory to an
        external object.
        @return
    */
    IFactory<T> *data()
    {
        return d.data();
    }

    /**
        @brief singleton
        A singleton for the factory itself.
        @return
    */
    static Factory<T, P> *singleton()
    {
        // In C++11, this is thread safe.
        static Factory<T, P> factory;
        return &factory;
    }

    /**
        @brief registerType
        Register a new pair between a TDerived class (derived from T),
        and a interface TInterface provided by TDerived.
        When factoryType is a pool the internal factory used is of type:
        - FactoryType::SMALL_POOL: ObjectPool<TDerived, DEFAULT_POOL_SIZE/4>
        - FactoryType::POOL: ObjectPool<TDerived>
        - FactoryType::BIG_POOL: ObjectPool<TDerived, DEFAULT_POOL_SIZE*4>
        @param factoryType
        @return false if the
    */
    template <typename TDerived, typename TInterface = TDerived>
    bool registerType(FactoryType factoryType = FactoryType::DEFAULT,
                      const QString &opName = QString())
    {
        bool bRet = false;
        QString ifName = getClassName<TInterface>();
        const auto isBase = ::std::is_base_of<TInterface GOAP_COMMA TDerived>::value;
        static_assert(isBase,
                          "Can't register class because it "
                          "does not implement interface.");
        bRet = registerByTypeName<TDerived>(ifName, opName, factoryType);
        if (!bRet)
        {
            qCritical() << __PRETTY_FUNCTION__ << " Can't register <" << ifName << ">(" << opName << "). Is it already registered?";
        }
        return bRet;
    }

    /**
        @brief isRegistered
        Test if the supplied interface is registered using the opName discriminator.
        @param opName
        @param ppolicy
        @return true if the interface is already registered.
    */
    template <typename TInterface>
    bool isRegistered(const QString &opName = QString(),
                      const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        const QString ifName = getClassName<TInterface>();
        const ICreationPolicy<T> *policy = policyByInterfaceName(ifName, opName);
        if (ppolicy)
        {
            *ppolicy = policy;
        }
        auto type = policy->factoryType();
        return type != FactoryType::INVALID;
    }

protected:

    /**
        @brief create
        Creates a new interface pointer `TInterface*` of a
        previously registered class.
        @param opName Optional operation name.
        @param pFactoryType Address of a FactoryType variable to be filled
        with a FactoryType enum value.
        Pass nullptr if no singleton information is needed.
        @return The pointer to the object interface or nullptr if no
        object is registered with the `TInterface` interface.
    */
    template <typename TInterface>
    TInterface *createRaw(const QString &opName = QString(),
                          const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        TInterface *ret = nullptr;
        const QString ifName = getClassName<TInterface>();
        const ICreationPolicy<T> *policy = policyByInterfaceName(ifName, opName);
        if (ppolicy)
        {
            *ppolicy = policy;
        }
        T *obj = policy->create();
        if (policy)
        {
            ret = dynamic_cast<TInterface *>(obj);
            if (ret == nullptr)
            {
                const QString tName = getClassName<T>();
                const QString pName = getClassName<P>();
                //const static QString errMsgStr = QStringLiteral(
                //    "The requested interface '%1' has not been registered "
                //    "in the Factory<%2, %3>.").arg(ifName).arg(tName).arg(pName);
                //Q_ASSERT_X(ret != nullptr, "Factory", qPrintable(errMsgStr));
                //throw FactoryException(errMsgStr);
                /// TODO: Use the logger
                qWarning() << "The requested interface '" << ifName
                           << "' has not been registered in the Factory<"
                           << tName << ", " << pName << ">.";
            }
        }
        return ret;
    }

public:
    /**
        @brief createAllSmart
        Returns a map betwen operation names and pointers to their instances.
        @return
    */
    template <typename TInterface>
    QMap<QString, TInterface *> createAll() const
    {
        QMap<QString, TInterface *> ret;
        QString ifName = getClassName<TInterface>();
        QStringList ops = d->getOperationsForInterface(ifName);
        for (auto it = ops.constBegin(); it != ops.constEnd(); ++it)
        {
            auto opName = *it;
            auto pIf = createRaw<TInterface>(opName);
            ret[opName] = pIf;
        }
        return ret;
    }

    template<typename TInterface>
    typename std::enable_if <has_goap_ref_counter<TInterface>::value,
             Cow<TInterface>>::type
             createCow(const QString &opName = QString(),
                       const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        auto p = createRaw<TInterface>(opName, ppolicy);
        return Cow<TInterface>(p);
    }

    template<typename TInterface>
    typename std::enable_if <has_goap_ref_counter<TInterface>::value,
             Ptr<TInterface>>::type
             createSmart(const QString &opName = QString(),
                         const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        auto p = createRaw<TInterface>(opName, ppolicy);
        return Ptr<TInterface>(p);
    }

    template<typename TInterface>
    bool create(QSharedDataPointer<TInterface> &ptr,
                const QString &opName = QString(),
                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        const ICreationPolicy<T> *policy = nullptr;
        TInterface *p = createRaw<TInterface>(opName, &policy);
        if (policy)
        {
            auto factoryType = policy->factoryType();
            if (factoryType == FactoryType::POOL)
            {
                auto errMsg = "Incompatible interface for Pool Factories.";
                Q_ASSERT_X(factoryType != FactoryType::POOL, "Factory", errMsg);
                // There is no way for hooking a generic QSharedDataPointer
                // referenced object so we'll give a fallback for release.
                //policy->recycle(ptr);
                //ptr = nullptr;
            }
        }
        if (ppolicy)
        {
            *ppolicy = policy;
        }
        ptr = QSharedDataPointer<TInterface>(p);
        return ptr;
    }

    template<typename TInterface>
    typename std::enable_if < !has_goap_ref_counter<TInterface>::value
    &&has_member_QAtomicInt__ref<TInterface>::value,
    QSharedDataPointer<TInterface >>::type
    createSmart(const QString &opName = QString(),
                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        QSharedDataPointer<TInterface> ptr;
        create(ptr, opName, ppolicy);
        return ptr;
    }

    template <template<class> class SharedPointer, class TInterface>
    typename std::enable_if <has_goap_ref_counter<TInterface>::value,
             void>::type createInner(SharedPointer<TInterface> &ptr, TInterface *p,
                                     const ICreationPolicy<T> *policy) const
    {
        Ptr<TInterface> pt(p);
        ptr = SharedPointer<TInterface>(p, [pt](TInterface *)
        {
            Q_UNUSED(pt); // Do nothing. Only capture the smart pointer
        });
    }

    template <template<class> class SharedPointer, class TInterface>
    typename std::enable_if < !has_goap_ref_counter<TInterface>::value,
             void >::type createInner(SharedPointer<TInterface> &ptr, TInterface *p,
                                      const ICreationPolicy<T> *policy) const
    {
        auto factoryType = policy->factoryType();
        bool isPool = factoryType == FactoryType::POOL
                      || factoryType == FactoryType::SMALL_POOL
                      || factoryType == FactoryType::BIG_POOL;
        if (isPool)
        {
            // If it is created from a pool, redirect the deletion to
            // the pool's recycling system.
            ptr = SharedPointer<TInterface>(p, [ = ](TInterface *)
            {
                policy->recycle(p);
            });
        }
        else
        {
            ptr = SharedPointer<TInterface>(p);
        }
    }

    template <template<class> class SharedPointer, class TInterface>
    bool create(SharedPointer<TInterface> &ptr,
                const QString &opName = QString(),
                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        const ICreationPolicy<T> *policy = nullptr;
        auto p = createRaw<TInterface>(opName, &policy);
        if (policy)
        {
            if (ppolicy)
            {
                *ppolicy = policy;
            }
            auto factoryType = policy->factoryType();
            if (factoryType == FactoryType::SINGLETON)
            {
                // If it is a singleton, remove the deletion.
                ptr = SharedPointer<TInterface>(p, [&](TInterface *) {});
            }
            else
            {
                createInner(ptr, p, policy);
            }
        }
        else
        {
            ptr = SharedPointer<TInterface>(p);
        }
        return p ? true : false;
    }

    /**
        @brief createSmart
        Creates the most appropriate smartpointer to store a reference counter
        to the registered object:
        - Ptr<TInterface>.- If the interface implements reference counting
         funcions.
        - QSharedDataPointer<TInterface>.- If the interface implements an
         QAtomicInt `ref` object.
        - QSharedPointer<TInterface>.- If the object does not fit the two
         previous. This is used by ordinary interfaces.
        @param pFactoryType Address of a FactoryType variable to be filled
        with a FactoryType enum value.
        Pass nullptr if no FactoryType information is needed.
        @return The most apropriate smart pointer to store the pointer to
        the object's interface or nullptr if no object is registered
        with the `TInterface` interface.
    */
    template<typename TInterface>
    typename std::enable_if < !has_goap_ref_counter<TInterface>::value
    &&!has_member_QAtomicInt__ref<TInterface>::value,
    QSharedPointer<TInterface >>::type
    createSmart(const QString &opName = QString(),
                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        QSharedPointer<TInterface> ptr;
        create(ptr, opName, ppolicy);
        return ptr;
    }

    // djg
    template<typename TInterface>
    bool create(TInterface  *&ptr,
                const QString &opName = QString(),
                const ICreationPolicy<T> **ppolicy = nullptr)
    {
        ptr = createRaw<TInterface>(opName, ppolicy);
        return ptr ? true : false;
    }

    template<typename TInterface>
    typename std::enable_if <has_goap_ref_counter<TInterface>::value,
             bool>::type create(Ptr<TInterface> &ptr,
                                const QString &opName = QString(),
                                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        ptr = Ptr<TInterface>(createRaw<TInterface>(opName, ppolicy));
        return ptr ? true : false;
    }

    template<typename TInterface>
    typename std::enable_if <has_goap_ref_counter<TInterface>::value,
             bool>::type create(Cow<TInterface> &ptr,
                                const QString &opName = QString(),
                                const ICreationPolicy<T> **ppolicy = nullptr) const
    {
        ptr = Cow<TInterface>(createRaw<TInterface>(opName, ppolicy));
        return ptr ? true : false;
    }

    /**
        @brief createAllSmart Returns a map betwen operation names and
        smart pointers to their instances.
        @return
    */
    template <typename TInterface = T>
    auto createAllSmart() const
    -> QMap < QString, decltype(this->createSmart<TInterface>(QString())) >
    {
        QMap < QString, decltype(this->createSmart<TInterface>(QString())) > ret;
        QString ifName = getClassName<TInterface>();
        QStringList ops = d->getOperationsForInterface(ifName);
        for (auto it = ops.constBegin(); it != ops.constEnd(); ++it)
        {
            auto opName = *it;
            auto pIf = createSmart<TInterface>(opName);
            ret[opName] = pIf;
        }
        return ret;
    }

private:

    Ptr<IFactory<T>> d;
};


/**
    @brief The RegisterTypesFunction type
    Function signature of any library function implementing custom registration.

    Any library whitch wants to register its `IFactoryItem` derived types,
    must implement the global function:
    int registerTypes(IFactory<IFactoryItem> * pFactory);

    The returned int is the number of successfully registered interfaces.

 *******************************************************************************

    A Typical use when loading a dll will pass the internal data from the
    current class factory:

    QLibrary library("qlibrary.dll");
    if (!library.load())
    qDebug() << library.errorString();
    if (library.load())
    qDebug() << "library loaded";

    typedef int (* RegisterTypesFunction)(IFactory<IFactoryItem> *);
    RegisterTypesFunction registerTypes =
        (RegisterTypesFunction)library.resolve("registerTypes");
    if (registerTypes)
    registerTypes(factoryItem()->data());
*/
int registerTypes(IFactory<IFactoryItem> *pFactory);
typedef int (* RegisterTypesFunction)(IFactory<IFactoryItem> *);


////////////////////////////////////////////////////////////////////////////////
/// Accessory functions for IFactoryItem derivatives
///

/**
    @brief factoryItem
    Helper function for retrieving a factory based on IFactoryItem.
    @return A default factory for objects derived from IFactoryItem.
*/
inline Factory<IFactoryItem> *factoryItem()
{
    return Factory<IFactoryItem>::singleton();
}

/**
    @brief factoryItemRegisterType
    Helper function for register a type based on IFactoryItem.
    @param factoryType True if the singleton pattern must be used
    when returning object interfaces. False is normal creation is used.
    @param opName Optional string to descrive a interface subclasification.
    @return true if registered successfully.
*/
template <typename TDerived, typename TInterface = TDerived>
bool factoryItemRegisterType(
    FactoryType factoryType = FactoryType::DEFAULT,
    const QString &opName = QString())
{
    return factoryItem()->registerType<TDerived, TInterface>(factoryType,
            opName);
}

/**
    @brief factoryItemCreate
    Creates a new interface pointer `TInterface*` of a
    previously registered class.
    @param opName Optional string to descrive a interface subclasification.
    @param pFactoryType Address of a FactoryType variable to be filled
    with a FactoryType enum value.
    Pass nullptr if no singleton information is needed.
    @return The pointer to the object interface or nullptr if no
    object is registered with the `TInterface` interface.
*/
template <typename TInterface>
TInterface *factoryItemCreateRaw(
    const QString &opName = QString(),
    const ICreationPolicy<IFactoryItem> **ppolicy = nullptr)
{
    return factoryItem()->createRaw<TInterface>(opName, ppolicy);
}

/**
    @brief factoryItemCreateSmart
    Creates the most appropriate smartpointer to store a reference counter
    to the registered object:
    - Ptr<TInterface>.- If the interface implements reference counting funcions.
    - QSharedDataPointer<TInterface>.- If the interface implements an
    QAtomicInt `ref` object.
    - QSharedPointer<TInterface>.- If the object does not fit the two previous.
    This is used by ordinary interfaces.
    @param opName Optional string to descrive a interface subclasification.
    @param pFactoryType Address of a boolean variable to be filled
    with true if the class was registered as a singleton, or false otherways.
    Pass nullptr if no singleton information is needed.
    @return The most apropriate smart pointer to store the pointer to
    the object's interface or nullptr if no object is registered
    with the `TInterface` interface.
*/
template <typename TInterface>
auto factoryItemCreateSmart(
    const QString &opName = QString(),
    const ICreationPolicy<IFactoryItem> **ppolicy = nullptr)
-> decltype(factoryItem()->createSmart<TInterface>())
{
    return factoryItem()->createSmart<TInterface>(opName, ppolicy);
}

/**
    @brief factoryItemCreateAllSmart
    Returns a collection mapping the registered op names with their related
    smart pointers.
*/
template <typename TInterface>
auto factoryItemCreateAllSmart()
-> decltype(factoryItem()->createAllSmart<TInterface>())
{
    return factoryItem()->createAllSmart<TInterface>();
}

////////////////////////////////////////////////////////////////////////////////
/// Factory hidders
///

/**
    @brief The HideFactory class
    This template class is a helper for creating value types based on
    smart pointers.
    This is usefull to hide the factory creation to the interface user.
    Instead of writting a factory creation line like:

    Ptr<IMyData> ptrData = factoryItem()->createSmart<IMyData>();

    you would write:

    PtrData ptrData;

    It's as easy as creating a value type (eg. like a QString).
    The PtrData is a typedef to improve readability of the real type HideFactory<IMyData>.

    This has another advantage: The smart pointer are not directly used in the interfaces.

    The HideFactory<> class is compatible with:
 * * QSmartPointer<>
 * * std::shared_ptr<>
 * * boost::shared_ptr<>
 * * goap:Ptr<>: Only if I has reference counting funcions
 * * goap:Cow<>: Only if I has reference counting funcions
*/
template<typename I, typename V, typename B>
class HideFactory
{
public:
    typedef V value_type; // V is an explicit smart pointer type
    value_type p;

    /**
        @brief HideFactory
        Factory creation.
        @param opName The String Discriminator used when registered the class
    */
    HideFactory(const QString &opName = QString())
    {
        Factory<B>::singleton()->create(p, opName);
        if (!p)
        {
            qWarning() << "HideFactory: Can't create an instance matching " << getClassName<I>() << "(" << opName << ")";
        }
    }
    /**
        @brief HideFactory
        Copy constructor
        @param o
    */
    HideFactory(const HideFactory<I, V, B> &o) : p(o.p)
    {
    }
    /**
        @brief HideFactory
        Create from a raw pointer.
        @param o
    */
    HideFactory(I *o) : p(o)
    {
    }
    template<typename T>
    inline HideFactory<I, V, B> &operator=(const T &o)
    {
        p = value_type(o);
        return *this;
    }
    template<typename T, typename W = T>
    inline typename std::enable_if <std::is_convertible<I *, T *>::value, HideFactory<T, W, B>>::type
            cast()
    {
        auto pt = static_cast<T *>(data());
        HideFactory<T, W, B> ptr(pt);
        return ptr;
    }
    template<typename T, typename W = T>
    inline typename std::enable_if < !std::is_convertible<I *, T *>::value, HideFactory<T, W, B >>::type
            cast()
    {
        auto pt = dynamic_cast<T *>(data());
        HideFactory<T, W, B> ptr(pt);
        return ptr;
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__get<W>::value && has_member_voidp__data<W>::value, I * >::type
    data()
    {
        return p.data();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__get<W>::value && !has_member_voidp__data<W>::value, I * >::type
    data()
    {
        return (I *)p;
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_voidp__get<W>::value, I *>::type
    data()
    {
        return p.get();
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_voidp__constData<W>::value, const I *>::type
    constData() const
    {
        return p.constData();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && has_member_voidp__get<W>::value, const I * >::type
    constData() const
    {
        return p.get();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && !has_member_voidp__get<W>::value && has_member_voidp__data<W>::value, const I * >::type
    constData() const
    {
        return p.data();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && !has_member_voidp__get<W>::value && !has_member_voidp__data<W>::value, const I * >::type
    constData() const
    {
        return (const I *)p;
    }
    inline I *operator->()
    {
        return data();
    }
    inline const I *operator->() const
    {
        return constData();
    }
    inline operator value_type &()
    {
        return p;
    }
    inline operator const value_type &() const
    {
        return p;
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_void__reset<W>::value, void>::type
    reset()
    {
        p.reset();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_void__reset<W>::value, void >::type
    reset()
    {
        p = nullptr;
    }
    inline operator bool () const
    {
        return constData() != nullptr;
    }
};

/**
    @brief The HideFactory<I, I> class
    Partial specialisation of the HideFactory<> template class.
    This template let the Factory decide the best smart pointer for storing
    the pointer to the interface 'I'.
    Currently the propossed interfaces from the factory are:
 * * goap:Ptr<>: Only if I has reference counting funcions
 * * QSmartPointer<>
 * * QSharedDataPointer<>
*/
template<typename I, typename B>
class HideFactory<I, I, B>
{
public:
    typedef decltype(Factory<B>::singleton()->template createSmart<I>()) value_type;
    value_type p;

    HideFactory(const QString &opName = QString()) :
        p(Factory<B>::singleton()->template createSmart<I>(opName))
    {
        if (!p)
        {
            qWarning() << "HideFactory: Can't create an instance matching " << getClassName<I>() << "(" << opName << ")";
        }
    }
    HideFactory(const HideFactory<I, I, B> &o) : p(o.p)
    {
    }
    HideFactory(I *o) : p(o)
    {
    }
    template<typename T>
    inline HideFactory<I, I, B> &operator=(const T &o)
    {
        p = value_type(o);
        return *this;
    }
    template<typename T>
    inline typename std::enable_if <std::is_convertible<I *, T *>::value, HideFactory<T, T, B>>::type
            cast()
    {
        auto pt = static_cast<T *>(data());
        return HideFactory<T, T, B>(pt);
    }
    template<typename T>
    inline typename std::enable_if < !std::is_convertible<I *, T *>::value, HideFactory<T, T, B>>::type
            cast()
    {
        auto pt = dynamic_cast<T *>(data());
        return HideFactory<T, T, B>(pt);
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__get<W>::value && has_member_voidp__data<W>::value, I * >::type
    data()
    {
        return p.data();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__get<W>::value && !has_member_voidp__data<W>::value, I * >::type
    data()
    {
        return (I *)p;
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_voidp__get<W>::value, I *>::type
    data()
    {
        return p.get();
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_voidp__constData<W>::value, const I *>::type
    constData() const
    {
        return p.constData();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && has_member_voidp__get<W>::value, const I * >::type
    constData() const
    {
        return p.get();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && !has_member_voidp__get<W>::value && has_member_voidp__data<W>::value, const I * >::type
    constData() const
    {
        return p.data();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_voidp__constData<W>::value && !has_member_voidp__get<W>::value && !has_member_voidp__data<W>::value, const I * >::type
    constData() const
    {
        return (const I *)p;
    }
    inline I *operator->()
    {
        return data();
    }
    inline const I *operator->() const
    {
        return constData();
    }
    inline operator value_type &()
    {
        return p;
    }
    inline operator const value_type &() const
    {
        return p;
    }
    template<typename W = value_type>
    inline typename std::enable_if <has_member_void__reset<W>::value, void>::type
    reset()
    {
        p.reset();
    }
    template<typename W = value_type>
    inline typename std::enable_if < !has_member_void__reset<W>::value, void >::type
    reset()
    {
        p = nullptr;
    }
    inline operator bool () const
    {
        return constData() != nullptr;
    }
};

}

#endif // GOAP_FACTORY_H
