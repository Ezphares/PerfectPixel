#pragma once

#include <EntityComponentSystem/EcsReflection.h>

#include <serialization/BinarySerializer.h>

#include <Bedrock/Hash.h>
#include <Bedrock/defines.h>
#include <Bedrock/numbers.h>
#include <EntityComponentSystem/Entity.h>

#include <type_traits>
#include <vector>

namespace perfectpixel { namespace ecs {

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
    virtual size_t size()                                   = 0;
};

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

template <typename Owner, typename OwnerRef, typename T>
class FieldImpl : public IField
{
private:
    struct FieldTypeInfo
    {
        int32_t ownerId;
        int32_t selfId;
    };

public:
    FieldImpl(FieldTable::ReflectionHint)
        : m_data()
        , m_default()
    {}

    FieldImpl(int32_t ownerId, int32_t selfId, int32_t typeId, T defaultValue)
        : m_data()
        , m_default(defaultValue)
    {
        m_typeInfo.ownerId = ownerId;
        m_typeInfo.selfId  = selfId;

        if (Owner::AddField(selfId, this))
        {
            FieldTable::getInstance()->add<Owner>(ownerId, selfId, typeId);
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
        T defaultValue)
        : m_data()
        , m_default(defaultValue)
    {
        m_typeInfo.ownerId = ownerId;
        m_typeInfo.selfId  = selfId;

        if (Owner::AddField(selfId, this))
        {
            FieldTable::getInstance()->add<Owner>(
                ownerName, ownerId, selfName, selfId, typeName, typeId);
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
        return m_data[Owner::Index(entity)];
    }

    void Set(Entity entity, const T &value)
    {
        m_data[Owner::Index(entity)] = value;
    }

    // Raw access operators
    T &operator()(Entity entity)
    {
        return m_data[Owner::Index(entity)];
    }
    const T &operator()(Entity entity) const
    {
        return m_data.at(Owner::Index(entity));
    }

    T &operator()(OwnerRef &ref)
    {
        DEBUG_ASSERT(Owner::Has(ref.m_entity));
        Owner::_fixRef(ref);
        return m_data[ref.m_index];
    }
    const T &operator()(OwnerRef &ref) const
    {
        DEBUG_ASSERT(Owner::Has(ref.m_entity));
        Owner::_fixRef(ref);
        return m_data.at(ref.m_index);
    }

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

    virtual size_t size() override
    {
        return sizeof(T);
    }

private:
    std::vector<T> m_data;
    T m_default;
    FieldTypeInfo m_typeInfo;
};

template <typename Owner, typename T, std::uint32_t Capacity>
class ArrayField : public IField
{
public:
    // FIXME Use a better container type
    typedef std::vector<T> Container;

    ArrayField(FieldTable::ReflectionHint)
        : m_data()
    {}

    ArrayField(int32_t ownerId, int32_t selfId, int32_t typeId)
        : m_data()
    {
        if (Owner::AddField(selfId, this))
        {
            FieldTable::getInstance()->add<Owner>(ownerId, selfId, typeId);
        }
    }

#if PP_FULL_REFLECTION_ENABLED
    ArrayField(
        const std::string &ownerName,
        int32_t ownerId,
        const std::string &selfName,
        int32_t selfId,
        const std::string &typeName,
        int32_t typeId)
        : m_data()
    {
        if (Owner::AddField(selfId, this))
        {
            FieldTable::getInstance()->add<Owner>(
                ownerName, ownerId, selfName, selfId, typeName, typeId);
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

    virtual size_t size() override
    {
        return sizeof(T) * Capacity;
    }

private:
    std::vector<std::vector<T>> m_data;
    T m_default;
};

}} // namespace perfectpixel::ecs
