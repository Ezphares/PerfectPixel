#pragma once

#include <EntityComponentSystem/Component.h>

#include <types/BitSet.h>

#include <vector>

namespace perfectpixel {
	namespace ecs {

		class Query
		{
		public:
			typedef void(*QueryFunction)(types::BitSet &);

			Query(QueryFunction func);

			void executeMaskOnly();
			void executeMaskOnly(const types::BitSet &start);
			std::vector<Entity> execute(EntityManager::EntityFunc callback = 0);
			std::vector<Entity> execute(const types::BitSet &start, EntityManager::EntityFunc callback = 0);
			std::vector<Entity> finalize(EntityManager::EntityFunc callback = 0);

			const types::BitSet &getLastResult() const;

		private:
			void applyMask();

		private:
			types::BitSet m_lastResult;
			QueryFunction m_queryFunction;
		};
	}
}