#pragma once

#include "worldgraph/Component.h"
#include "worldgraph/Entity.h"

#include "types/BitSet.h"

namespace perfectpixel
{
	namespace world
	{
		class IComponentStorageBase
		{
		public:
			virtual ~IComponentStorageBase(){}

			virtual void clean() = 0;
			virtual types::BitSet getMask(ComponentTypeId selector, const types::BitSet &hint) const = 0;
		};

		template<typename T>
		class IComponentStorage : public IComponentStorageBase
		{
		public:
			virtual ~IComponentStorage() {}

			virtual T *getComponent(Entity entity) = 0;

			virtual void registerComponent(const T &component) = 0;
			virtual void removeComponent(const T &component) = 0;
		};
	}
}