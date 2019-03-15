#pragma once

#include <behaviour/Behaviour.h>

#include <worldgraph/Entity.h>
#include <worldgraph/EntityManager.h>

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
				world::EntityManager *entityManager,
				api::ManagerInterface *managerInterface);
			~BehaviourManager();

		public:
			void registerBehaviour(world::Entity entity, Behaviour *component);

			void update(types::PpFloat deltaT);

			template<typename T>
			T *getBehaviour(world::Entity entity)
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

				return NULL;
			}

		private:
			void registerBehaviourInternal(world::Entity entity, Behaviour *component);
			void destroyBehaviour(Behaviour *behaviour);

		private:
			typedef std::map< world::Entity, std::vector<Behaviour*>> BehaviourList;

			std::vector<Behaviour*> m_created;
			std::vector<std::pair<world::Entity, Behaviour*>> m_deferredCreate;

			std::set<world::Entity> m_toDestroyAll;
			std::set<Behaviour*> m_toDestroySingle;

			world::EntityManager *m_entityManager;
			api::ManagerInterface *m_managerInterface;

			BehaviourList m_behaviours;

			bool m_updating;
		};

	}
}
