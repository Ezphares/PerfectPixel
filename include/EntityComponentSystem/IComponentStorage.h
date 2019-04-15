#pragma once

#include "EntityComponentSystem/Component.h"
#include "EntityComponentSystem/Entity.h"

#include "types/BitSet.h"

namespace perfectpixel
{
	namespace ecs
	{
		class IComponentStorageBase
		{
		public:
			virtual ~IComponentStorageBase(){}

			virtual void clean() = 0;
			virtual types::BitSet getMask(ComponentTypeId selector, const types::BitSet &hint) const = 0;
			virtual bool hasComponent(Entity entity) const = 0;
		};

		template<typename T>
		class IComponentStorage : public IComponentStorageBase
		{
		public:
			virtual ~IComponentStorage() {}

			virtual T *getComponent(Entity entity) = 0;

			virtual void registerComponent(const T &component) = 0;
			virtual void removeComponent(Entity entity) = 0;
		};
	}
}