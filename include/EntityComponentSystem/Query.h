#pragma once

#include <EntityComponentSystem/Component.h>

#include <Bedrock/BitSet.h>

#include <vector>

namespace perfectpixel { namespace ecs {

class Query
{
public:
    typedef void (*QueryFunction)(bedrock::BitSet &);

    Query(QueryFunction func);

    void executeMaskOnly();
    void executeMaskOnly(const bedrock::BitSet &start);
    EntityManager::EntityList execute(EntityManager::EntityFunc callback = 0);
    EntityManager::EntityList execute(
        const bedrock::BitSet &start, EntityManager::EntityFunc callback = 0);
    EntityManager::EntityList finalize(EntityManager::EntityFunc callback = 0);

    const bedrock::BitSet &getLastResult() const;

private:
    void applyMask();

private:
    bedrock::BitSet m_lastResult;
    QueryFunction m_queryFunction;
};
}} // namespace perfectpixel::ecs