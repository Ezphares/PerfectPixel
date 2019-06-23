#pragma once

#include <EntityComponentSystem/IComponentStorage.h>

#include <map>
#include <queue>

namespace perfectpixel { namespace ecs {

	// Stores an array of idx->Entity and looks up using linear scan
	// Useful for semi uncommon components
	class LinearScanComponentStorage : public IComponentStorage
	{
	public:
		LinearScanComponentStorage()
			: m_maxEntityIndex(0u)
			, m_entities()
		{}

		virtual bool _has(Entity entity) const
		{
			for (uint32_t i = 0u; i < m_entities.size(); ++i)
			{
				if (m_entities[i] == entity)
				{
					return true;
				}
			}
			return false;
		}

		virtual uint32_t _index(Entity entity) const
		{
			for (uint32_t i = 0u; i < m_entities.size(); ++i)
			{
				if (m_entities[i] == entity)
				{
					return i;
				}
			}
			return false;
		}

		virtual uint32_t _register(Entity entity, uint32_t currentSize)
		{
			uint32_t eidx = entityIndex(entity);
			if (eidx > m_maxEntityIndex)
			{
				m_maxEntityIndex = eidx;
			}

			uint32_t idx = m_entities.size();
			m_entities.push_back(entity);
			return idx;
		}

		virtual uint32_t _delete(Entity entity)
		{
			for (uint32_t i = 0u; i < m_entities.size(); ++i)
			{
				if (m_entities[i] == entity)
				{
					m_entities.erase(m_entities.begin() + i);
					return i;
				}
			}
			return 0;
		}

		virtual void _filter(types::BitSet &mask, ComponentStorageFilterType filterType) const
		{
			types::BitSet localMask = types::BitSet(m_maxEntityIndex + 1, filterType == IComponentStorage::WITHOUT);
			for (auto entity : m_entities)
			{
				localMask[entityIndex(entity)] = (filterType == IComponentStorage::WITH);
			}

			mask &= localMask;
		}

	private:
		std::uint32_t m_maxEntityIndex;
		std::vector<Entity> m_entities;
	};

	// Stores component data at the same index as the entity
	// Useful for VERY common components
	template<typename T>
	class FlatComponentStorage : public IComponentStorage
	{
	public:
		virtual bool _has(Entity entity) const
		{
			uint32_t idx = entityIndex(entity);
			return m_mask.size() > idx && m_mask[idx] && T::at(idx) == entity;
		}

		virtual uint32_t _index(Entity entity) const
		{
			return entityIndex(entity);
		}

		virtual uint32_t _register(Entity entity, uint32_t currentSize)
		{
			uint32_t idx = entityIndex(entity);
			if (m_mask.size() <= idx)
			{
				m_mask.resize(idx + 1);
			}
			m_mask[idx] = true;
			return idx;
		}
		virtual uint32_t _delete(Entity entity)
		{
			m_mask[entityIndex(entity)] = false;
			return entityIndex(entity);
		}

		virtual void _filter(types::BitSet &mask, ComponentStorageFilterType filterType) const
		{
			mask &= (filterType == IComponentStorage::WITH) ? m_mask : ~m_mask;
		}

	private:
		types::BitSet m_mask;
	};

	// Component storage using a map entity->index
	// Useful for VERY rare component types
	class MapComponentStorage : public IComponentStorage
	{
	public:
		MapComponentStorage()
			: IComponentStorage()
			, m_maxEntityIndex(0u)
			, m_freeIndices()
			, m_indices()
		{}

		virtual bool _has(Entity entity) const { return m_indices.find(entity) != m_indices.end(); }
		virtual uint32_t _index(Entity entity) const { return m_indices.find(entity)->second; }
		virtual uint32_t _register(Entity entity, uint32_t currentSize)
		{
			uint32_t eidx = entityIndex(entity);
			if (eidx > m_maxEntityIndex)
			{
				m_maxEntityIndex = eidx;
			}

			if (m_freeIndices.empty())
			{
				m_indices[entity] = currentSize;
				return currentSize;
			}
			else
			{
				uint32_t idx = m_freeIndices.front();
				m_indices[entity] = idx;
				m_freeIndices.pop();
				return idx;
			}
		}

		virtual uint32_t _delete(Entity entity)
		{
			auto it = m_indices.find(entity);
			uint32_t idx = it->second;
			m_freeIndices.push(idx);
			m_indices.erase(it);
			return idx;
		}

		virtual void _filter(types::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType) const
		{
			types::BitSet localMask = types::BitSet(m_maxEntityIndex + 1, filterType == IComponentStorage::WITHOUT);

			for (auto it : m_indices)
			{
				localMask[entityIndex(it.first)] = (filterType == IComponentStorage::WITH);
			}

			mask &= localMask;
		}

	private:
		std::uint32_t m_maxEntityIndex;
		std::queue<uint32_t> m_freeIndices;
		std::map<Entity, uint32_t> m_indices;
	};

	typedef LinearScanComponentStorage DefaultComponentStorage;
} }
