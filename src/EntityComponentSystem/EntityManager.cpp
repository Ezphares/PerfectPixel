#include <EntityComponentSystem/EntityManager.h>

namespace perfectpixel {
	namespace ecs {

EntityManager::EntityManager(
	std::uint32_t indexReuseDelay)
	: m_indexReuseDelay(indexReuseDelay)
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
	return m_entities[entityIndex(entity)] == entity;
}

void EntityManager::kill(Entity entity)
{
	if (isAlive(entity))
	{
		uint32_t index = entityIndex(entity);
		m_entities[index] = entityCreate(entityGeneration(entity) + 1, index);
		m_indexReuse.push(index);
	}
}

Entity EntityManager::at(std::uint32_t index)
{
	if (index > m_entities.size())
	{
		return -1;
	}
	return m_entities[index];
}

void EntityManager::expandMask(types::BitSet bits, std::vector<Entity> *out_entities, EntityFunc callback)
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

types::BitSet EntityManager::all() const
{
	return types::BitSet(m_entities.size(), true);
}

}}
