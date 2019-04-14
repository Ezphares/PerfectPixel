#pragma once

#include <worldgraph/Entity.h>

#include <types/BitSet.h>

#include <functional>

#include <vector>
#include <queue>

namespace perfectpixel {
	namespace world {

		class EntityManager
		{
		public:
			typedef std::function<void(Entity)> EntityFunc;

		public:
			EntityManager(
				std::uint32_t indexReuseDelay = 1024);
			~EntityManager();

		public:
			Entity create();
			bool isAlive(Entity entity);
			void kill(Entity entity);

			Entity at(std::uint32_t index);

			void expandMask(types::BitSet bits, std::vector<Entity> *out_entities, EntityFunc callback);
			types::BitSet all() const;

		private:
			std::uint32_t m_indexReuseDelay;
			std::vector<Entity> m_entities;
			std::queue<uint32_t> m_indexReuse;
		};

	}
}
