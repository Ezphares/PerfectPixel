#pragma once

#include <EntityComponentSystem/ReflectionTable.h>

#include <serialization/BinarySerializer.h>

#include <Bedrock/Hash.h>
#include <Bedrock/defines.h>
#include <Bedrock/numbers.h>
#include <EntityComponentSystem/Entity.h>

#include <array>
#include <type_traits>
#include <vector>

namespace perfectpixel { namespace ecs {

template <typename T, typename U>
concept Accessor = requires(T a, U b)
{
    a = b;
    b = a;
};

class IField
{
public:
    typedef bool (*SerializationCondition)(Entity);

    virtual void reset(uint32_t index = 0) = 0;
    virtual void
    serialize(serialization::ISerializer &serializer, uint32_t index)
        = 0;
    virtual void
    deserialize(serialization::ISerializer &serializer, uint32_t index)
        = 0;
    virtual void copy(uint32_t dstIndex, uint32_t srcIndex) = 0;
    virtual void setBuffer(void *buffer)                    = 0;
    // virtual void setManager(void *manager)                  = 0;
};

typedef bool (*FieldAdder)(int32_t, IField *);
typedef void (*LUTFiller)(ComponentLUTEntry &);
typedef uint32_t (*Indexer)(Entity);

inline bool doNotSerialize(Entity)
{
    return false;
}

template <typename Owner>
struct SerializationRule
{
    SerializationRule(int32_t fieldId, IField::SerializationCondition condition)
    {
        Owner::AddSerializationRule(fieldId, condition);
    }
};

template <typename T, Accessor<T> TAccessor = T &>
class FieldImpl : public IField
{
private:
    typedef T TValue;

    struct FieldTypeInfo
    {
        int32_t ownerId;
        int32_t selfId;
    };

public:
    FieldImpl()
        : m_data()
        , m_default()
    {}

    FieldImpl(ReflectionTable::ReflectionHint)
        : m_data()
        , m_default()
    {}

    FieldImpl(
        int32_t ownerId,
        int32_t selfId,
        int32_t typeId,
        T defaultValue,
        FieldAdder fieldCallback,
        LUTFiller lutCallback,
        Indexer indexCallback)
        : m_data()
        , m_default(defaultValue)
        , m_indexCallback(indexCallback)
    {
        m_typeInfo.ownerId = ownerId;
        m_typeInfo.selfId  = selfId;

        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerId, selfId, typeId, lutEntry);
        }
    }

#if PP_FULL_REFLECTION_ENABLED
    FieldImpl(
        const std::string &ownerName,
        int32_t ownerId,
        const std::string &selfName,
        int32_t selfId,
        const std::string &typeName,
        int32_t typeId,
        T defaultValue,
        FieldAdder fieldCallback,
        LUTFiller lutCallback,
        Indexer indexCallback)
        : m_data()
        , m_default(defaultValue)
        , m_indexCallback(indexCallback)
    {
        m_typeInfo.ownerId = ownerId;
        m_typeInfo.selfId  = selfId;

        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerName,
                ownerId,
                selfName,
                selfId,
                typeName,
                typeId,
                lutEntry);
        }
    }
#endif

    T at(uint32_t idx)
    {
        return m_data[idx];
    }

    void _set(uint32_t idx, const T &val)
    {
        m_data[idx] = val;
    }

    T Get(Entity entity) const
    {
        return m_data[m_indexCallback(entity)];
    }

    void Set(Entity entity, const T &value)
    {
        m_data[m_indexCallback(entity)] = value;
    }

    // Raw access operators
    TAccessor &operator()(Entity entity)
    {
        return m_data[m_indexCallback(entity)];
    }
    const TAccessor &operator()(Entity entity) const
    {
        return m_data.at(m_indexCallback(entity));
    }

    // T &operator()(OwnerRef &ref)
    //{
    //    DEBUG_ASSERT(Owner::Has(ref.m_entity));
    //    Owner::_fixRef(ref);
    //    return m_data[ref.m_index];
    //}
    // const T &operator()(OwnerRef &ref) const
    //{
    //    DEBUG_ASSERT(Owner::Has(ref.m_entity));
    //    Owner::_fixRef(ref);
    //    return m_data.at(ref.m_index);
    //}

    virtual void reset(uint32_t index) override
    {
        if (m_data.size() <= index)
        {
            m_data.resize(index + 1, m_default);
        }
        m_data[index] = m_default;
    }

    virtual void
    serialize(serialization::ISerializer &serializer, uint32_t index) override
    {
        serializer << m_data[index];
    }

    virtual void
    deserialize(serialization::ISerializer &serializer, uint32_t index) override
    {
        _deserialize<T>(serializer, index);
    }

    virtual void copy(uint32_t dstIndex, uint32_t srcIndex) override
    {
        m_data[dstIndex] = m_data[srcIndex];
    }

    // For overloading reasons we have to split the deserialization into enums
    // and everything else
    template <
        typename X,
        typename std::enable_if_t<!std::is_enum<X>::value> * = nullptr>
    void _deserialize(serialization::ISerializer &serializer, uint32_t index)
    {
        serializer >> m_data[index];
    }

    template <
        typename X,
        typename std::enable_if_t<std::is_enum<X>::value> * = nullptr>
    void _deserialize(serialization::ISerializer &serializer, uint32_t index)
    {
        typename std::underlying_type<T>::type temp;
        serializer >> temp;
        m_data[index] = static_cast<T>(temp);
    }

    virtual void setBuffer(void *buffer) override
    {
        m_buffer = reinterpret_cast<TValue *>(buffer);
    }

private:
    std::vector<T> m_data;
    T m_default;
    FieldTypeInfo m_typeInfo;
    TValue *m_buffer;
    Indexer m_indexCallback;
};

template <typename Owner, typename T, std::uint32_t Capacity>
class ArrayField : public IField
{
public:
    // FIXME Use a better container type
    typedef std::vector<T> Container;

    typedef T TValue;
    typedef TValue TContainer[Capacity];

    ArrayField(ReflectionTable::ReflectionHint)
        : m_data()
    {}

    ArrayField(
        int32_t ownerId,
        int32_t selfId,
        int32_t typeId,
        FieldAdder fieldCallback,
        LUTFiller lutCallback,
        Indexer indexCallback)
        : m_data()
        , m_indexCallback(indexCallback)
    {
        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerId, selfId, typeId, lutEntry);
        }
    }

#if PP_FULL_REFLECTION_ENABLED
    ArrayField(
        const std::string &ownerName,
        int32_t ownerId,
        const std::string &selfName,
        int32_t selfId,
        const std::string &typeName,
        int32_t typeId,
        FieldAdder fieldCallback,
        LUTFiller lutCallback,
        Indexer indexCallback)
        : m_data()
        , m_indexCallback(indexCallback)
    {
        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerName,
                ownerId,
                selfName,
                selfId,
                typeName,
                typeId,
                lutEntry);
        }
    }
#endif

    T at(uint32_t idx)
    {
        return m_data[idx];
    }

    const T &get(Entity entity) const
    {
        return m_data[Owner::Index(entity)];
    }

    void set(Entity entity, const T &value)
    {
        m_data[Owner::Index(entity)] = value;
    }

    uint32_t capacity(Entity entity)
    {
        return m_data[Owner::Index(entity)].size();
    }

    // Raw access operator
    Container &operator()(Entity entity)
    {
        return m_data[Owner::Index(entity)];
    }

    T &operator()(Entity entity, uint32_t index)
    {
        return m_data[Owner::Index(entity)][index];
    }

    const Container &operator()(Entity entity) const
    {
        return m_data.at(Owner::Index(entity));
    }

    const T &operator()(Entity entity, uint32_t index) const
    {
        return m_data.at(Owner::Index(entity)).at(index);
    }

    virtual void reset(uint32_t idx) override
    {
        if (m_data.size() <= idx)
        {
            m_data.resize(idx + 1);
        }
        m_data[idx] = std::vector<T>();
    }

    virtual void
    serialize(serialization::ISerializer &serializer, uint32_t index) override
    {
        serializer.writeArrayStart();

        for (const T &element : m_data[index])
        {
            (void)element;
            //				serializer << element; TODO
        }

        serializer.writeArrayEnd();
    }

    virtual void
    deserialize(serialization::ISerializer &serializer, uint32_t index) override
    {
        uint32_t arraySize = std::min(serializer.readArrayStart(), Capacity);

        m_data[index].resize(arraySize);

        // TODO:

        serializer.readArrayEnd();
    }

    virtual void copy(uint32_t dstIndex, uint32_t srcIndex) override
    {
        m_data[dstIndex] = m_data[srcIndex];
    }

    virtual void setBuffer(void *buffer) override
    {
        m_buffer = reinterpret_cast<TContainer *>(buffer);
    }

private:
    std::vector<std::vector<T>> m_data;
    T m_default;
    TContainer *m_buffer;
    Indexer m_indexCallback;
};

template <typename T>
struct FieldUnderlying
{
    typedef T Type;
};

template <typename T, std::uint32_t Capacity>
struct FieldContainer
{
    typedef std::array<typename FieldUnderlying<T>::Type, Capacity> Type;
};

template <typename T>
struct FieldContainer<T, 1u>
{
    typedef typename FieldUnderlying<T>::Type Type;
};

class IFieldDescriptor
{
public:
    virtual void instantiate(IField *instance, void *manager, void *buffer) = 0;
    virtual std::size_t size()                                              = 0;
};

template <typename T, Accessor<T> TAccessor = T &, uint32_t Capacity = 1u>
class FieldDescriptor : public IFieldDescriptor
{
public:
    typedef bool (*FieldDescAdder)(int32_t, IFieldDescriptor *);

    FieldDescriptor(
        int32_t ownerId,
        int32_t selfId,
        int32_t typeId,
        T defaultValue,
        FieldDescAdder fieldCallback,
        LUTFiller lutCallback)
        : m_id(selfId)
        , m_default(defaultValue)
    {
        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerId, selfId, typeId, lutEntry);
        }
    }

#if PP_FULL_REFLECTION_ENABLED
    FieldDescriptor(
        const std::string &ownerName,
        int32_t ownerId,
        const std::string &selfName,
        int32_t selfId,
        const std::string &typeName,
        int32_t typeId,
        T defaultValue,
        FieldDescAdder fieldCallback,
        LUTFiller lutCallback)
        : m_id(selfId)
        , m_default(defaultValue)
    {
        if (fieldCallback(selfId, this))
        {
            ComponentLUTEntry lutEntry;
            lutCallback(lutEntry);
            ReflectionTable::getInstance()->add(
                ownerName,
                ownerId,
                selfName,
                selfId,
                typeName,
                typeId,
                lutEntry);
        }
        m_default = defaultValue;
    }
#endif

    virtual void
    instantiate(IField *instance, void *manager, void *buffer) override
    {
        (void)instance;
        (void)manager;
        (void)buffer;
        // if constexpr (Capacity < 2)
        //{
        //    instance = new FieldImpl<Owner, OwnerRef, T, TAccessor>();
        //}
        // else
        //{
        //    instance = new ArrayField<Owner, T, Capacity>();
        //}
        // instance->setBuffer(buffer);
        // instance->setManager(manager);
    }

    virtual std::size_t size() override
    {
        if constexpr (Capacity < 2)
        {
            return sizeof(T);
        }
        else
        {
            return sizeof(T) * Capacity;
        }
    }

private:
    int32_t m_id;
    T m_default;
};

}} // namespace perfectpixel::ecs
