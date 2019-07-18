#pragma once

#include <EntityComponentSystem/Entity.h>
#include <Bedrock/BitSet.h>
#include <Bedrock/Singleton.h>

#include <functional>

#include <vector>
#include <queue>

namespace perfectpixel {
	namespace ecs {

		class EntityManager : public bedrock::Singleton<EntityManager>
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

			void expandMask(bedrock::BitSet bits, std::vector<Entity> *out_entities, EntityFunc callback);
			bedrock::BitSet all() const;

			void addKillCallback(EntityFunc callback);

		private:
			std::uint32_t m_indexReuseDelay;
			std::vector<Entity> m_entities;
			std::queue<uint32_t> m_indexReuse;

			std::vector<EntityFunc> m_onKill;
		};

	}
}
