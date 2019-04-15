#pragma once

#include "managerinterface/ManagerInterface.h"

#include "EntityComponentSystem/Component.h"

#include "types/BitSet.h"

namespace perfectpixel {
	namespace api {

		class Query
		{
		public:
			Query();

			Query &with(world::ComponentTypeId id);
			Query &without(world::ComponentTypeId id);

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
			std::vector<world::Entity> execute(world::EntityManager::EntityFunc callback = 0);
			std::vector<world::Entity> execute(const types::BitSet &start, world::EntityManager::EntityFunc callback = 0);
			std::vector<world::Entity> finalize(world::EntityManager::EntityFunc callback = 0);

			const types::BitSet &getLastResult() const;

		private:
			void applyMask();

		private:
			std::vector<world::ComponentTypeId> m_with;
			std::vector<world::ComponentTypeId> m_without;
			types::BitSet m_lastResult;
		};
	}
}