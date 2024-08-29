#pragma once

#include <type_traits>

#include "bedrock/Hash.h"
#include "bedrock/TypeReflection.h"

namespace perfectpixel::core {

struct ResourceKey
{
    bedrock::TypeID type;
    bedrock::ID id;
};

struct ResourceLoaderKey
{
    bedrock::TypeID type;
    bedrock::TypeID variant;
};

} // namespace perfectpixel::core

namespace std {

bool operator==(
    const perfectpixel::core::ResourceKey &lhs,
    const perfectpixel::core::ResourceKey &rhs);

template <>
struct hash<perfectpixel::core::ResourceKey>
{
    size_t operator()(const perfectpixel::core::ResourceKey &key) const
    {
        if constexpr (
            sizeof(perfectpixel::bedrock::TypeID) < sizeof(size_t)
            && (sizeof(perfectpixel::bedrock::Hash) < sizeof(size_t)))
        {
            return ((size_t)key.type) | (((size_t)key.id.m_hash) << 32);
        }
        else
        {
            return ((size_t)key.type) ^ ((size_t)key.id.m_hash);
        }
    }
};

bool operator==(
    const perfectpixel::core::ResourceLoaderKey &lhs,
    const perfectpixel::core::ResourceLoaderKey &rhs);

template <>
struct hash<perfectpixel::core::ResourceLoaderKey>
{
    size_t operator()(const perfectpixel::core::ResourceLoaderKey &key) const
    {
        if constexpr (sizeof(perfectpixel::bedrock::TypeID) < sizeof(size_t))
        {
            return ((size_t)key.type) | (((size_t)key.variant) << 32);
        }
        else
        {
            return ((size_t)key.type) ^ ((size_t)key.variant);
        }
    }
};

} // namespace std