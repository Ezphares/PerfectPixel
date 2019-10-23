#include <EntityComponentSystem/EntityManager.h>

namespace perfectpixel {
	namespace ecs {

EntityManager::EntityManager(
	std::uint32_t indexReuseDelay)
	: m_indexReuseDelay(indexReuseDelay)
	, m_tickIndex(0)
{
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::create()
{
	uint32_t index = m_entities.size();
	if (m_indexReuse.size() > m_indexReuseDelay)
	{
		index = m_indexReuse.front();
		m_indexReuse.pop();
		// Generation has been increased by kill in this case
	}
	else
	{
		m_entities.push_back(entityCreate(0, index));
	}

	return m_entities[index];
}

bool EntityManager::isAlive(Entity entity)
{
	return m_entities[entity.index] == entity;
}

void EntityManager::kill(Entity entity)
{
	if (isAlive(entity))
	{
		for (auto callback : m_onKill)
		{
			callback(entity);
		}
		uint32_t index = entity.index;
		m_entities[index] = entityCreate(entity.generation + 1, index);
		m_indexReuse.push(index);
	}
}

Entity EntityManager::at(std::uint32_t index)
{
	if (index > m_entities.size())
	{
		return entityCreate(~0u, ~0u);
	}
	return m_entities[index];
}

void EntityManager::expandMask(
    bedrock::BitSet bits, EntityList *out_entities, EntityFunc callback) const
{
	if (out_entities) out_entities->clear();

	for (std::size_t i = 0; i < bits.size(); ++i)
	{
		if (i >= m_entities.size())
		{
			return;
		}

		if (bits[i])
		{
			if (callback) callback(m_entities[i]);
			if (out_entities) out_entities->push_back(m_entities[i]);
		}
	}
}

bedrock::BitSet EntityManager::all() const
{
	return bedrock::BitSet(m_entities.size(), true);

}

void EntityManager::addKillCallback(EntityFunc callback)
{
	m_onKill.push_back(callback);
}

void EntityManager::tick()
{
    ++m_tickIndex;
}

uint32_t EntityManager::getTick() const
{
    return m_tickIndex;
}

}}
