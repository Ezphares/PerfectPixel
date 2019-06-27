#pragma once

#include "EntityComponentSystem/Component.h"
#include "EntityComponentSystem/Entity.h"

#include "types/BitSet.h"

namespace perfectpixel{ namespace ecs {

	class IComponentStorage
	{
	public:
		enum ComponentStorageFilterType
		{
			WITH,
			WITHOUT,
		};

		virtual bool _has(Entity entity) const = 0;
		virtual uint32_t _index(Entity entity) const = 0;
		virtual uint32_t _register(Entity entity, uint32_t currentSize) = 0;
		virtual uint32_t _delete(Entity entity) = 0;
		virtual uint32_t _safeDelete(Entity entity) = 0;
		virtual void _filter(types::BitSet &mask, ComponentStorageFilterType filterType) const = 0;
		virtual void _clean() = 0;
	};
	
}}
