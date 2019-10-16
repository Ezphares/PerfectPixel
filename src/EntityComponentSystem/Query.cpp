#include <EntityComponentSystem/Query.h>

namespace perfectpixel { namespace ecs {

Query::Query(
    QueryFunction queryFunction, DirtyCheck dirtyCheck)
    : m_lastResult()
    , m_queryFunction(queryFunction)
    , m_isDirty(dirtyCheck)
{}

void Query::applyMask()
{
    if (m_queryFunction != nullptr)
    {
        m_queryFunction(m_lastResult);
    }
}

bool Query::defaultDirtyCheck() { return true; }

void Query::executeMaskOnly()
{
	executeMaskOnly(EntityManager::getInstance()->all());
}

void Query::executeMaskOnly(const bedrock::BitSet &start, bool ignoreDirtyCheck /* = false */)
{
	if (ignoreDirtyCheck || isDirty())
	{
		m_lastResult = start;
		applyMask();
	}
}

EntityManager::EntityList Query::execute(EntityManager::EntityFunc callback)
{
    executeMaskOnly();
    return finalize(callback);
}

EntityManager::EntityList
Query::execute(const bedrock::BitSet &start, EntityManager::EntityFunc callback)
{
    executeMaskOnly(start);
    return finalize(callback);
}

EntityManager::EntityList
Query::finalize(EntityManager::EntityFunc callback /*= 0*/)
{
    EntityManager::EntityList result;
    EntityManager::getInstance()->expandMask(m_lastResult, &result, callback);
    return result;
}

const perfectpixel::bedrock::BitSet &Query::getLastResult() const
{
    return m_lastResult;
}

bool Query::isDirty() const 
{ 
	return m_isDirty == nullptr || m_isDirty(); 
}

}} // namespace perfectpixel::ecs
