#pragma once

#include <EntityComponentSystem/IComponentStorage.h>

namespace perfectpixel {
	namespace ecs {

		template <typename ComponentType>
		class HintComponentContainer : public IComponentStorage<ComponentType>
		{
		public:
			HintComponentContainer()
				: m_bitSet()
				, m_hint(0)
			{
			}
			virtual ~HintComponentContainer() {}

			virtual types::BitSet getMask(ComponentTypeId selector, const types::BitSet &hint) const
			{
				return m_bitSet;
			}

			virtual void clean()
			{
				// Hints MUST be maintained manually
			}

			virtual ComponentType *getComponent(Entity entity)
			{
				return hasComponent(entity) ? &m_hint : nullptr;
			}

			virtual bool hasComponent(Entity entity) const
			{
				uint32_t index{ entityIndex(entity) };
				return m_bitSet.size() > index && m_bitSet[entityIndex(entity)] == true;
			}

			virtual void registerComponent(const ComponentType &component)
			{
				uint32_t index{ entityIndex(component.m_entity) };

				if (m_bitSet.size() <= index)
				{
					m_bitSet.resize(index + 1);
				}
				m_bitSet[index] = true;
			}

			virtual void removeComponent(Entity entity)
			{
				uint32_t index{ entityIndex(entity) };
				if (m_bitSet.size() > index)
				{
					m_bitSet[index] = false;
				}
			}


		private:
			types::BitSet m_bitSet;
			ComponentType m_hint;
			
		};
} }
