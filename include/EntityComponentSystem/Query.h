#pragma once

#include "managerinterface/ManagerInterface.h"

#include "EntityComponentSystem/Component.h"

#include "types/BitSet.h"

namespace perfectpixel {
	namespace ecs {

		class Query
		{
		public:
			Query();

			Query &with(ComponentTypeId id);
			Query &without(ComponentTypeId id);

			template<typename Component>
			Query &with()
			{
				return with(Component::getTypeId());
			}

			template<typename Without>
			Query &without()
			{
				return without(Component::getTypeId());
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