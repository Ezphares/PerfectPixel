#pragma once

#include <EntityComponentSystem/Component.h>

#include <Bedrock/BitSet.h>

#include <vector>

namespace perfectpixel { namespace ecs {

class Query
{
public:
    typedef void (*QueryFunction)(bedrock::BitSet &);
    typedef bool (*DirtyCheck)();

    Query(QueryFunction func, DirtyCheck dirtyCheck = &defaultDirtyCheck);

    void executeMaskOnly();
    void executeMaskOnly(const bedrock::BitSet &start, bool ignoreDirtyCheck = false);
    EntityManager::EntityList execute(EntityManager::EntityFunc callback = 0);
    EntityManager::EntityList execute(
        const bedrock::BitSet &start, EntityManager::EntityFunc callback = 0);
    EntityManager::EntityList finalize(EntityManager::EntityFunc callback = 0);

    const bedrock::BitSet &getLastResult() const;
    bool isDirty() const;

private:
    void applyMask();
	
	static bool defaultDirtyCheck();

private:
    bedrock::BitSet m_lastResult;
    QueryFunction m_queryFunction;
    DirtyCheck m_isDirty;
};
}} // namespace perfectpixel::ecs