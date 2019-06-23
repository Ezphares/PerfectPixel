#pragma once

#include "managerinterface/ManagerInterface.h"

#include <EntityComponentSystem/Component.h>

#include <types/BitSet.h>

#include <vector>

namespace perfectpixel {
	namespace ecs {

		class Query
		{
		public:
			typedef void(*QueryFunction)(types::BitSet &);

			Query(EntityManager *entityManager, QueryFunction func);

			void executeMaskOnly();
			void executeMaskOnly(const types::BitSet &start);
			std::vector<Entity> execute(EntityManager::EntityFunc callback = 0);
			std::vector<Entity> execute(const types::BitSet &start, EntityManager::EntityFunc callback = 0);
			std::vector<Entity> finalize(EntityManager::EntityFunc callback = 0);

			const types::BitSet &getLastResult() const;
			EntityManager *getEntityManager() const { return m_entityManager; }

		private:
			void applyMask();

		private:
			types::BitSet m_lastResult;
			QueryFunction m_queryFunction;
			EntityManager *m_entityManager;
		};
	}
}