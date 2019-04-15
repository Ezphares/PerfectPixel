#pragma once

#include <behaviour/Behaviour.h>

#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/EntityManager.h>

#include <managerinterface/ManagerInterface.h>

#include <map>
#include <vector>
#include <set>

namespace perfectpixel {
	namespace behaviour {

		class BehaviourManager
		{
		public:
			BehaviourManager(
				ecs::EntityManager *entityManager,
				api::ManagerInterface *managerInterface);
			~BehaviourManager();

		public:
			void registerBehaviour(ecs::Entity entity, Behaviour *component);

			void update(types::PpFloat deltaT);

			template<typename T>
			T *getBehaviour(ecs::Entity entity)
			{
				auto &it = m_behaviours.find(entity);
				if (it != m_behaviours.end())
				{
					for (Behaviour *component : it->second)
					{
						T *attempt = dynamic_cast<T*>(component);
						if (attempt)
						{
							return attempt;
						}
					}
				}

				return nullptr;
			}

			template <typename T>
			types::BitSet getMask(const types::BitSet &reference) const
			{
				// FIXME optimize
				std::vector<ecs::Entity> entities;
				m_entityManager->expandMask(reference, entities);

				types::BitSet result{ reference.size() };

				for (auto it : entities)
				{
					if (getBehaviour<T>(it) != nullptr)
					{
						result.set(ecs::entityIndex(it), true);
					}
				}

				return result;
			}

		private:
			void registerBehaviourInternal(ecs::Entity entity, Behaviour *component);
			void destroyBehaviour(Behaviour *behaviour);

		private:
			typedef std::map< ecs::Entity, std::vector<Behaviour*>> BehaviourList;

			std::vector<Behaviour*> m_created;
			std::vector<std::pair<ecs::Entity, Behaviour*>> m_deferredCreate;

			std::set<ecs::Entity> m_toDestroyAll;
			std::set<Behaviour*> m_toDestroySingle;

			ecs::EntityManager *m_entityManager;
			api::ManagerInterface *m_managerInterface;

			BehaviourList m_behaviours;

			bool m_updating;
		};

	}
}
