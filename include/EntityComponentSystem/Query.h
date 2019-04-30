#pragma once

#include "managerinterface/ManagerInterface.h"

#include <EntityComponentSystem/Component.h>

#include "types/BitSet.h"

namespace perfectpixel {
	namespace ecs {

		class Query
		{
		public:
			Query();

			Query &with(ComponentTypeId id);
			Query &without(ComponentTypeId id);

			template<typename ComponentType>
			Query &with()
			{
				return with(ComponentType::getTypeId());
			}

			template<typename ComponentType>
			Query &without()
			{
				return without(ComponentType::getTypeId());
			}

			void executeMaskOnly();
			void executeMaskOnly(const types::BitSet &start);
			std::vector<Entity> execute(EntityManager::EntityFunc callback = 0);
			std::vector<Entity> execute(const types::BitSet &start, EntityManager::EntityFunc callback = 0);
			std::vector<Entity> finalize(EntityManager::EntityFunc callback = 0);

			const types::BitSet &getLastResult() const;

		private:
			void applyMask();

		private:
			std::vector<ComponentTypeId> m_with;
			std::vector<ComponentTypeId> m_without;
			types::BitSet m_lastResult;
		};
	}
}