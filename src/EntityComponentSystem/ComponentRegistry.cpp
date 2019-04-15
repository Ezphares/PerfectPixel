#include "EntityComponentSystem/ComponentRegistry.h"

namespace perfectpixel
{
	namespace world {

		ComponentRegistry *ComponentRegistry::m_instance = nullptr;


		ComponentRegistry::ComponentRegistry(EntityManager *entityManager)
			: m_storageLocator()
			, m_managedStorage()
			, m_entityManager(entityManager)
		{
			if (m_instance != nullptr)
			{
				delete m_instance;
			}
			m_instance = this;
		}


		ComponentRegistry::~ComponentRegistry()
		{
			for (auto it : m_managedStorage)
			{
				delete it;
			}
		}

		perfectpixel::world::ComponentRegistry * ComponentRegistry::Instance()
		{
			if (m_instance == nullptr)
			{
				throw "ComponentRegistry not initialized";
			}

			return m_instance;
		}

		void ComponentRegistry::registerStorage(ComponentTypeId id, IComponentStorageBase *storage, bool managed)
		{
			m_storageLocator[id] = storage;
			if (managed)
			{
				m_managedStorage.push_back(storage);
			}
		}


		world::EntityManager * ComponentRegistry::getEntityManager()
		{
			return m_entityManager;
		}

	}
}