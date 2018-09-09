#pragma once

#include <worldgraph/Entity.h>

#include <vector>
#include <queue>

namespace perfectpixel {
	namespace world {

		class EntityManager
		{
		public:
			EntityManager(
				std::uint32_t indexReuseDelay = 1024);
			~EntityManager();

		public:
			Entity create();
			bool isAlive(Entity entity);
			void kill(Entity entity);

		private:
			std::uint32_t m_indexReuseDelay;
			std::vector<Entity> m_entities;
			std::queue<uint32_t> m_indexReuse;
		};

	}
}
