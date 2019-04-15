#include <behaviour/BehaviourManager.h>


namespace perfectpixel {
	namespace behaviour {

		BehaviourManager::BehaviourManager(
			ecs::EntityManager *entityManager,
			api::ManagerInterface *managers)
			: m_entityManager(entityManager)
			, m_managerInterface(managers)
			, m_updating(false)
		{
		}


		BehaviourManager::~BehaviourManager()
		{
		}

		void BehaviourManager::registerBehaviour(ecs::Entity entity, Behaviour *component)
		{
			component->m_managerInterface = m_managerInterface;
			m_deferredCreate.emplace_back(entity, component);
		}

		void BehaviourManager::update(types::PpFloat deltaT)
		{
			// The lifecycle has to be update -> create -> destroy
			// to ensure that the create script has been run before other managers pick up the entity
			m_updating = true;


			// UPDATE
			for (auto &it : m_behaviours)
			{
				for (Behaviour *beh : it.second)
				{
					if (!m_entityManager->isAlive(beh->getEntity()))
					{
						beh->m_state = Behaviour::LC_DEAD;
						m_toDestroyAll.insert(beh->getEntity());
					}

					if (beh->m_state == Behaviour::LC_ACTIVE)
					{
						beh->onUpdate(deltaT);
					}
				}
			}

			// CREATE
			// Run this in a loop to ensure that behaviours that create others in their onCreate gets registered
			while (!m_deferredCreate.empty())
			{
				for (auto it : m_deferredCreate)
				{
					registerBehaviourInternal(it.first, it.second);
				}
				m_deferredCreate.clear();

				for (auto beh : m_created)
				{
					beh->onCreate();

					// State check because the behaviour might already be dead. We still want to run onCreate, as onDestroy might depend on it
					if (beh->m_state == Behaviour::LC_NEW)
					{
						beh->m_state = Behaviour::LC_ACTIVE;
					}
				}
				m_created.clear();
			}

			// DESTROY
			for (auto entity : m_toDestroyAll)
			{
				auto it = m_behaviours.find(entity);
				if (it == m_behaviours.end())
				{
					continue;
				}

				for (auto beh : it->second)
				{
					m_toDestroySingle.erase(beh); // Already destroyed, we do not want to do it again
					destroyBehaviour(beh);
				}

				// Clear out entity from registry
				m_behaviours.erase(it);
			}
			m_toDestroyAll.clear();

			for (auto beh : m_toDestroySingle)
			{
				destroyBehaviour(beh);
				
				// Detach from entity
				auto it = m_behaviours.find(beh->getEntity());
				if (it != m_behaviours.end())
				{
					for (auto jt = it->second.begin(); jt != it->second.end();)
					{
						if (*jt == beh)
						{
							jt = it->second.erase(jt);
						}
						else 
						{
							jt++;
						}
					}
				}

			}
			m_toDestroySingle.clear();

			m_updating = false;
		}

		void BehaviourManager::registerBehaviourInternal(ecs::Entity entity, Behaviour *component)
		{
			m_behaviours[entity].push_back(component);
			m_created.push_back(component);
		}

		void BehaviourManager::destroyBehaviour(Behaviour *behaviour)
		{
			behaviour->onDestroy();
			delete behaviour;
		}

	}
}
