#pragma once

#include "bedrock/BitSet.h"
#include "ecs/Entity.h"

namespace perfectpixel { namespace ecs {

enum class ComponentStorageFilterType
{
    WITH,
    WITHOUT,
};

template <typename T>
concept ComponentStorage = requires(const T &cref, Entity entity, bool _)
{
    _ = cref._has(entity);
}
&&requires(const T &cref, Entity entity, uint32_t _)
{
    _ = cref._index(entity);
}
&&requires(const T &cref, uint32_t index, Entity _)
{
    _ = cref._at(index);
}
&&requires(T &ref, Entity entity, uint32_t size, uint32_t _)
{
    _ = ref._register(entity, size);
}
&&requires(T &ref, Entity entity, uint32_t _)
{
    _ = ref._delete(entity);
}
&&requires(T &ref, Entity entity, uint32_t _)
{
    _ = ref._safeDelete(entity);
}
&&requires(
    const T &cref, bedrock::BitSet &mask, ComponentStorageFilterType filterType)
{
    cref._filter(mask, filterType);
}
&&requires(T &ref)
{
    ref._clean();
};

}} // namespace perfectpixel::ecs
