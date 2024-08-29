#include "core/ResourceKeys.h"

bool std::operator==(
    const perfectpixel::core::ResourceKey &lhs,
    const perfectpixel::core::ResourceKey &rhs)
{
    return lhs.id == rhs.id && lhs.type == rhs.type;
}

bool std::operator==(
    const perfectpixel::core::ResourceLoaderKey &lhs,
    const perfectpixel::core::ResourceLoaderKey &rhs)
{
    return lhs.type == rhs.type && lhs.variant == rhs.variant;
}
