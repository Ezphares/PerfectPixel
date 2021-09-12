#pragma once

#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/FieldHelper.h>
#include <EntityComponentSystem/IComponentStorage.h>

#include <Bedrock/BitSet.h>
#include <Bedrock/Singleton.h>

#include <map>

namespace perfectpixel { namespace ecs {

template <typename T, ComponentStorage TStorage>
class Component : public bedrock::Singleton<T>, public TStorage
{
public:
    typedef Component<T, TStorage> ComponentType;

    // Sub-classing Reference by component improves type-safety by letting the
    // compiler error if attempts to cross-use references are made
    struct Reference : public UntypedReference
    {
        Reference(Entity entity = NO_ENTITY, uint32_t index = ~0u)
            : UntypedReference(entity, index)
        {}
    };

    static Component<T, TStorage>
    createManager(Entity group, uint32_t capacity, uint8_t *data = nullptr)
    {
        (void)group;
        if (data == nullptr)
        {
            data = malloc(getInstance()->m_size * capacity);
        }

        for (auto field : getInstance()->m_fields)
        {
            // TODO
            // field->second->create(capacity, data);
            data += field->second->size() * capacity;
        }
    }

    using bedrock::Singleton<T>::getInstance;

protected:
    Component()
        : m_objects(0)
        , m_lastIndex(0)
        , m_fields()
        , m_dirtyFrame(0)
        , m_size(0)
        , m_serializationRules()
    {}

private:
    uint32_t m_objects;
    uint32_t m_lastIndex;
    std::map<int32_t, IField *> m_fields;
    uint32_t m_dirtyFrame;
    size_t m_size;
    std::map<int32_t, IField::SerializationCondition> m_serializationRules;

protected:
    PPTransientField(ComponentType, Entity, _Entity);

    virtual void purge(uint32_t idx)
    {
        (void)idx;
    };
    virtual void initialize(uint32_t idx)
    {
        for (auto field : m_fields)
        {
            field.second->reset(idx);
        }
    };

public:
    static Entity at(uint32_t idx)
    {
        return _Entity.at(idx);
    }

    static bool AddField(int32_t id, IField *field)
    {
        auto &fields = getInstance()->m_fields;
        // Inline statics are weird, we have to check for duplicates
        bool add = fields.find(id) == fields.end();
        if (add)
        {
            fields[id] = field;
            getInstance()->m_size += field->size();
        }

        return add;
    }

    static void
    AddSerializationRule(int32_t id, IField::SerializationCondition rule)
    {
        getInstance()->m_serializationRules[id] = rule;
    }

    static IField *Lookup(int32_t id)
    {
        return getInstance()->m_fields[id];
    }

    static bool Has(Entity entity)
    {
        return getInstance()->_has(entity);
    }

    static uint32_t Index(Entity entity)
    {
        return getInstance()->_index(entity);
    }

    static Reference Register(Entity entity)
    {
        T *self      = getInstance();
        uint32_t idx = self->_register(entity, self->m_lastIndex);

        if (idx >= self->m_lastIndex)
        {
            self->m_lastIndex++;
        }

        _Entity.reset(idx);
        _Entity._set(idx, entity);
        self->initialize(idx);
        self->m_objects++;

        self->m_dirtyFrame = EntityManager::getInstance()->getTick();

        return Reference(entity, idx);
    }

    // Reference-discarding version of register to be used for lookups
    static void VoidRegister(Entity entity)
    {
        Register(entity);
    }

    static Reference GetRef(Entity entity)
    {
        DEBUG_ASSERT(Has(entity));

        return Reference(entity, Index(entity));
    }

    static void Delete(Entity entity)
    {
        uint32_t idx = getInstance()->_delete(entity);

        getInstance()->purge(idx);

        getInstance()->m_dirtyFrame = EntityManager::getInstance()->getTick();

        getInstance()->m_objects--;
    }

    static void Copy(Entity destination, Entity source)
    {
        if (!Has(destination))
        {
            Register(destination);
        }

        uint32_t dstIndex = Index(destination);
        uint32_t srcIndex = Index(source);

        T *self = getInstance();
        for (auto field : self->m_fields)
        {
            field.second->copy(dstIndex, srcIndex);
        }
    }

    static void SafeDelete(Entity entity)
    {
        if (getInstance()->_has(entity))
            Delete(entity);
    }

    static void
    Filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType)
    {
        getInstance()->_filter(mask, filterType);
    }

    static void SetToDefault(Entity entity)
    {
        T *instance = getInstance();
        instance->initialize(instance->_index(entity));
    }

    static void Clean()
    {
        getInstance()->_clean();
    }

    static void Serialize(serialization::ISerializer &serializer, Entity entity)
    {
        if (Has(entity))
        {
            serializer.writeMapStart();

            auto &fields = getInstance()->m_fields;
            auto &rules  = getInstance()->m_serializationRules;
            uint32_t idx = Index(entity);

            for (auto it = fields.begin(); it != fields.end(); ++it)
            {
                auto condition = rules.find(it->first);
                if (condition != rules.end() && !condition->second(entity))
                {
                    continue;
                }

#if PP_FULL_REFLECTION_ENABLED
                serializer.writeMapKey(
                    FieldTable::getInstance()->reverse(it->first));
#else
                serializer.writeMapKey(it->first);
#endif
                it->second->serialize(serializer, idx);
            }

            serializer.writeMapEnd();
        }
    }

    static void
    Deserialize(serialization::ISerializer &serializer, Entity entity)
    {
        Register(entity);
        uint32_t idx = Index(entity);

        auto fields = getInstance()->m_fields;
        int32_t k;

        while (serializer.readMapKey(&k))
        {
            if (serializer.isValueNull())
                continue;

            fields[k]->deserialize(serializer, idx);
        }
    }

    static uint32_t DirtyFrame()
    {
        return getInstance()->m_dirtyFrame;
    }

    static inline void _fixRef(Reference &ref)
    {
        if (ref.m_index == ~0u)
        {
            ref.m_index = Index(ref.m_entity);
        }
    }
};

class HintComponentStorage
{
public:
    bool _has(Entity entity) const
    {
        uint32_t idx = entity.index;
        return m_mask.size() > idx && m_mask[idx];
    }

    uint32_t _index(Entity entity) const
    {
        return entity.index;
    }

    uint32_t _register(Entity entity, uint32_t currentSize)
    {
        (void)currentSize;

        uint32_t idx = entity.index;
        if (m_mask.size() <= idx)
        {
            m_mask.resize(idx + 1);
        }
        m_mask[idx] = true;
        return idx;
    }

    uint32_t _delete(Entity entity)
    {
        m_mask[entity.index] = false;
        return 0;
    }

    uint32_t _safeDelete(Entity entity)
    {
        return _delete(entity);
    }

    void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
    {
        mask &= (filterType == ComponentStorageFilterType::WITH) ? m_mask
                                                                 : ~m_mask;
    }

    void _clean()
    {}

protected:
    bedrock::BitSet m_mask;
};

template <typename T>
class HintComponent : public Component<T, HintComponentStorage>
{
public:
    inline static bedrock::BitSet &Mask()
    {
        return bedrock::Singleton<T>::getInstance()->m_mask;
    }
};

}} // namespace perfectpixel::ecs
