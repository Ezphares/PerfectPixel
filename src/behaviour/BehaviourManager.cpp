#include <behaviour/BehaviourManager.h>


namespace perfectpixel {
	namespace behaviour {

		BehaviourManager::BehaviourManager(world::EntityManager *entityManager)
			: m_entityManager(entityManager)
			, m_updating(false)
		{
		}


		BehaviourManager::~BehaviourManager()
		{
		}

		void BehaviourManager::registerBehaviour(world::Entity entity, Behaviour *behaviour)
		{
			if (!m_updating)
			{
				m_behaviours[entity].push_back(behaviour);
				m_created.push_back(behaviour);
			}
			else
			{
				m_deferredCreate.emplace_back(entity, behaviour);
			}
		}

		void BehaviourManager::update(types::PpFloat deltaT)
		{
			m_updating = true;

			// Run all onCreate
			for (auto beh : m_created)
			{
				beh->onCreate();
			}
			m_created.clear();

			// Update loop
			for (auto &it : m_behaviours)
			{
				for (Behaviour *beh : it.second)
				{
					beh->onUpdate(deltaT);
					if (!m_entityManager->isAlive(beh->getEntity()))
					{
						m_toDestroyAll.insert(beh->getEntity());
					}
				}
			}

			// Run all destroy
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

			// Run deferred actions
			for (auto it : m_deferredCreate)
			{
				registerBehaviour(it.first, it.second);
			}
			m_deferredCreate.clear();
		}

		void BehaviourManager::destroyBehaviour(Behaviour *behaviour)
		{
			behaviour->onDestroy();
			delete behaviour;
		}

	}
}
