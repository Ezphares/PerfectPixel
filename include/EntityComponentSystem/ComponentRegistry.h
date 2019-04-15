#pragma once

#include "EntityComponentSystem/Component.h"
#include "EntityComponentSystem/IComponentStorage.h"
#include "EntityComponentSystem/GenericComponentStorage.h"

#include <map>
#include <vector>

namespace perfectpixel {
	namespace ecs {

		class ComponentRegistry
		{
		public:
			ComponentRegistry(EntityManager *entityManager);
			virtual ~ComponentRegistry();

			// TODO: This is a singleton for now, but should be 1 per Game instance
			static ComponentRegistry *Instance();

			template<typename T>
			IComponentStorage<T> *registerGenericStorage()
			{
				ComponentTypeId id = T::getTypeId();
				if (m_storageLocator.find(id) != m_storageLocator.end())
				{
					throw "Storage already exists for type id";
				}

				IComponentStorage<T> *storage = new GenericComponentStorage<T>(m_entityManager);
				registerStorage(id, storage, true);
				return storage;
			}

			template<typename T>
			IComponentStorage<T> *getStorage()
			{
				return dynamic_cast<IComponentStorage<T>*>(getStorage(T::getTypeId()));
			}

			IComponentStorageBase *getStorage(ComponentTypeId id)
			{
				auto it = m_storageLocator.find(id);
				if (it == m_storageLocator.end())
				{
					return nullptr;
				}
				return it->second;
			}

			template <typename T>
			T *getComponent(Entity entity)
			{
				return getStorage<T>()->getComponent(entity);
			}

			template <typename T>
			void registerComponent(const T& component)
			{
				getStorage<T>()->registerComponent(component);
			}

			void registerStorage(ComponentTypeId id, IComponentStorageBase *storage, bool managed = false);

			EntityManager *getEntityManager();

		private:
			std::map<ComponentTypeId, IComponentStorageBase*> m_storageLocator;
			std::vector<IComponentStorageBase*> m_managedStorage;
			EntityManager *m_entityManager;

			static ComponentRegistry *m_instance;
		};
	}
}
