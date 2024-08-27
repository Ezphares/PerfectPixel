#pragma once

#include "ecs/ComponentLUTEntry.h"
#include "ecs/CoreComponentStorage.h"
#include "ecs/EntityManager.h"
#include "ecs/FieldHelper.h"

#include "bedrock/BitSet.h"
#include "bedrock/Singleton.h"

#include <concepts>
#include <map>

namespace perfectpixel { namespace ecs {

class BaseComponent
{
public:
    BaseComponent();
    virtual ~BaseComponent();

    void initialize(uint32_t idx);
    void destroy(uint32_t idx);
    virtual void prepare(uint32_t idx);
    virtual void purge(uint32_t idx);

    bool addField(int32_t id, IField *field);
    void addSerializationRule(int32_t id, IField::SerializationCondition rule);
    IField *lookup(int32_t id);

    void serialize(
        serialization::ISerializer &serializer, Entity entity, uint32_t idx);
    void deserialize(serialization::ISerializer &serializer, uint32_t idx);

    std::size_t size();

protected:
    uint32_t m_objects;
    uint32_t m_lastIndex;
    std::map<int32_t, IField *> m_fields;
    uint32_t m_dirtyFrame;
    std::map<int32_t, IField::SerializationCondition> m_serializationRules;
};

class ManagerRegistration
{
public:
    typedef void (*Factory)(BaseComponent *, Entity, uint32_t, void *);
    typedef size_t (*Sizer)();

    ManagerRegistration(
        int32_t id, Factory factory, Sizer sizer, uint32_t count)
    {
        MetaData meta;
        meta.factory          = factory;
        meta.sizer            = sizer;
        meta.count            = count;
        m_defaultManagers[id] = meta;
    }

private:
    struct MetaData
    {
        Factory factory;
        Sizer sizer;
        uint32_t count;
    };

    std::map<int32_t, MetaData> m_defaultManagers;
};

#define PPDefaultManager(T, Capacity)                                          \
    inline static ManagerRegistration __Registration = ManagerRegistration(    \
        PP_ID(Component), &T::InitManager, &T::Size, Capacity)

template <typename T, ComponentStorage TStorage = DefaultComponentStorage>
class Component : public bedrock::Singleton<T>,
                  public BaseComponent,
                  protected TStorage
{
public:
    typedef Component<T, TStorage> ComponentType;

    // Sub-classing Reference by component improves type-safety by letting
    // the compiler error if attempts to cross-use references are made
    struct Reference : public UntypedReference
    {
        Reference(Entity entity = NO_ENTITY, uint32_t index = ~0u)
            : UntypedReference(entity, index)
        {}
    };

    using bedrock::Singleton<T>::getInstance;

protected:
    Component()
        : BaseComponent()
    {}

public:
    static bool AddField(int32_t id, IField *field)
    {
        static_assert(std::is_base_of_v<ComponentType, T>);
        return getInstance()->addField(id, field);
    }
    static void
    AddSerializationRule(int32_t id, IField::SerializationCondition rule)
    {
        return getInstance()->addSerializationRule(id, rule);
    }

    static IField *Lookup(int32_t id)
    {
        return getInstance()->lookup(id);
    }

    static bool Has(Entity entity)
    {
        return getInstance()->_has(entity);
    }

    static bool Has(Entity entity, T *manager)
    {
        return manager->_has(entity);
    }

    static uint32_t Index(Entity entity)
    {
        return getInstance()->_index(entity);
    }

    static Reference Register(Entity entity)
    {
        DEBUG_ASSERT(!Has(entity));

        T *self      = getInstance();
        uint32_t idx = self->_register(entity, self->m_lastIndex);
        self->prepare(idx);
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
        getInstance()->destroy(idx);
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
            uint32_t idx = Index(entity);
            getInstance()->serialize(serializer, entity, idx);
        }
    }

    static void
    Deserialize(serialization::ISerializer &serializer, Entity entity)
    {
        if (!getInstance()->_has(entity))
            Register(entity);

        uint32_t idx = Index(entity);
        getInstance()->deserialize(serializer, idx);
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

    static std::size_t Size()
    {
        return getInstance()->size();
    }

    static void FillLUTEntry(ComponentLUTEntry &lutEntry)
    {
        lutEntry.m_has         = &ComponentType::Has;
        lutEntry.m_serialize   = &ComponentType::Serialize;
        lutEntry.m_deserialize = &ComponentType::Deserialize;
        lutEntry.m_register    = &ComponentType::VoidRegister;
        lutEntry.m_delete      = &ComponentType::Delete;
        lutEntry.m_fields      = &ComponentType::Lookup;
        lutEntry.m_copy        = &ComponentType::Copy;
    }
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
