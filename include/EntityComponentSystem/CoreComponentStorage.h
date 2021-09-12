#pragma once

#include <EntityComponentSystem/ComponentStorageConcept.h>

#include <queue>
#include <unordered_map>

namespace perfectpixel { namespace ecs {

// Stores an array of idx->Entity and looks up using linear scan
// Useful for semi uncommon components
class LinearScanComponentStorage
{
public:
    LinearScanComponentStorage();

    bool _has(Entity entity) const;
    uint32_t _index(Entity entity) const;
    Entity _at(uint32_t index) const;
    uint32_t _register(Entity entity, uint32_t currentSize);
    uint32_t _delete(Entity entity);
    uint32_t _safeDelete(Entity entity);

    void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const;

    void _clean();

private:
    std::uint32_t m_maxEntityIndex;
    std::vector<Entity> m_entities;
};

// Stores component data at the same index as the entity
// Useful for VERY common components
class FlatComponentStorage
{
public:
    FlatComponentStorage();

    bool _has(Entity entity) const;
    uint32_t _index(Entity entity) const;
    Entity _at(uint32_t index) const;
    uint32_t _register(Entity entity, uint32_t currentSize);
    uint32_t _delete(Entity entity);
    uint32_t _safeDelete(Entity entity);

    void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const;

    void _clean();

private:
    bedrock::BitSet m_mask;
    std::vector<Entity> m_entities;
};

// Component storage using a map entity->index
class MapComponentStorage
{
public:
    MapComponentStorage();

    bool _has(Entity entity) const;
    uint32_t _index(Entity entity) const;
    Entity _at(uint32_t index) const;
    uint32_t _register(Entity entity, uint32_t currentSize);
    uint32_t _delete(Entity entity);
    uint32_t _safeDelete(Entity entity);

    void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const;

    void _clean();

private:
    std::uint32_t m_maxEntityIndex;
    std::queue<uint32_t> m_freeIndices;
    std::unordered_map<Entity, uint32_t> m_indices;
};

typedef MapComponentStorage DefaultComponentStorage;
}} // namespace perfectpixel::ecs
