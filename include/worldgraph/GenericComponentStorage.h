#pragma once

#include "worldgraph/IComponentStorage.h"
#include "worldgraph/EntityManager.h"

#include <map>

namespace perfectpixel {
	namespace world {

		template<typename T>
		class GenericComponentStorage : public IComponentStorage<T>
		{
		public:
			typedef std::map<Entity, T> ComponentContainerType;

			GenericComponentStorage(EntityManager *entityManager)
				: m_components()
				, m_entityManager(entityManager)
				, m_bitSet()
			{}

			virtual ~GenericComponentStorage() {};

			virtual T *getComponent(Entity entity)
			{
				// Fast bitset pre-check
				uint32_t index = entityIndex(entity);
				if (index >= m_bitSet.size() || m_bitSet.get(index) == false)
				{
					return nullptr;
				}

				auto it = m_components.find(entity);

				return it != m_components.end() ? &(it->second) : nullptr;
			}

			virtual types::BitSet getMask(ComponentTypeId selector, const types::BitSet &hint) const
			{
				if (selector == T::getTypeId())
				{
					return m_bitSet;
				}
				else return types::BitSet();
			}

			virtual void registerComponent(const T &component)
			{
				uint32_t index = entityIndex(component.m_entity);
				if (m_bitSet.size() <= index)
				{
					m_bitSet.resize(index + 1);
				}
				m_bitSet.set(index, true);

				m_components.insert_or_assign(component.m_entity, component);
			}

			virtual void removeComponent(const T &component)
			{
				auto it = m_components.find(component.m_entity);
				if (it != m_components.end())
				{
					m_components.erase(it);

					uint32_t index = entityIndex(component.m_entity);
					if (index < m_components.size())
					{
						m_bitSet.set(index, false);
					}
				}
			}

			virtual void clean()
			{
				std::vector<T*> toRemove;

				for (auto it : m_components)
				{
					if (!m_entityManager->isAlive(it.first))
					{
						toRemove.push_back(&(it.second));
					}
				}

				for (auto it : toRemove)
				{
					removeComponent(*it);
				}
			}

		private:
			ComponentContainerType m_components;
			EntityManager *m_entityManager;
			types::BitSet m_bitSet;
		};

	}
}