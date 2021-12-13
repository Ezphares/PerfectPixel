#pragma once

#include <Bedrock/BitSet.h>
#include <Bedrock/Singleton.h>
#include <EntityComponentSystem/Entity.h>

#include <queue>
#include <vector>

namespace perfectpixel { namespace ecs {

struct UntypedReference
{
    inline UntypedReference(Entity entity = NO_ENTITY, uint32_t index = ~0u)
        : m_entity(entity)
        , m_index(index)
    {}

    Entity m_entity;
    uint32_t m_index;
};

class EntityManager : public bedrock::Singleton<EntityManager>
{
public:
    typedef void (*EntityFunc)(Entity);
    typedef std::vector<Entity> EntityList;

public:
    EntityManager(std::uint32_t indexReuseDelay = 1024);
    ~EntityManager();

public:
    Entity create();
    bool isAlive(Entity entity);
    void kill(Entity entity);

    Entity at(std::uint32_t index);

    void expandMask(
        bedrock::BitSet bits,
        EntityList *out_entities,
        EntityFunc callback) const;
    bedrock::BitSet all() const;

    void addKillCallback(EntityFunc callback);

    void tick();
    uint32_t getTick() const;

private:
    std::uint32_t m_indexReuseDelay;
    std::vector<Entity> m_entities;
    std::queue<uint32_t> m_indexReuse;

    std::vector<EntityFunc> m_onKill;

    uint32_t m_tickIndex;
};

}} // namespace perfectpixel::ecs
