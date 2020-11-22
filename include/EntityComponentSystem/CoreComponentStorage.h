#pragma once

#include <EntityComponentSystem/IComponentStorage.h>

#include <queue>
#include <unordered_map>

namespace perfectpixel { namespace ecs {

// Stores an array of idx->Entity and looks up using linear scan
// Useful for semi uncommon components
class LinearScanComponentStorage : public IComponentStorage
{
public:
    LinearScanComponentStorage()
        : m_maxEntityIndex(0u)
        , m_entities()
    {}

    virtual bool _has(Entity entity) const
    {
        for (uint32_t i = 0u; i < m_entities.size(); ++i)
        {
            if (m_entities[i] == entity)
            {
                return true;
            }
        }
        return false;
    }

    virtual uint32_t _index(Entity entity) const
    {
        for (uint32_t i = 0u; i < m_entities.size(); ++i)
        {
            if (m_entities[i] == entity)
            {
                return i;
            }
        }
        return false;
    }

    virtual uint32_t _register(Entity entity, uint32_t currentSize)
    {
        (void)currentSize; // Not needed in LinearScan

        uint32_t eidx = entity.index;
        if (eidx > m_maxEntityIndex)
        {
            m_maxEntityIndex = eidx;
        }

        uint32_t idx = m_entities.size();
        m_entities.push_back(entity);
        return idx;
    }

    virtual uint32_t _delete(Entity entity)
    {
        for (uint32_t i = 0u; i < m_entities.size(); ++i)
        {
            if (m_entities[i] == entity)
            {
                m_entities.erase(m_entities.begin() + i);
                return i;
            }
        }
        return 0;
    }

    virtual uint32_t _safeDelete(Entity entity)
    {
        return _delete(entity);
    }

    virtual void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
    {
        bedrock::BitSet localMask = bedrock::BitSet(
            m_maxEntityIndex + 1, filterType == IComponentStorage::WITHOUT);
        for (auto entity : m_entities)
        {
            localMask[entity.index] = (filterType == IComponentStorage::WITH);
        }

        mask &= localMask;
    }

    virtual void _clean()
    {
        for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
        {
            EntityManager *em = EntityManager::getInstance();
            if (!em->isAlive(*it))
            {
                it = m_entities.erase(it);
            }
        }
    }

private:
    std::uint32_t m_maxEntityIndex;
    std::vector<Entity> m_entities;
};

// Stores component data at the same index as the entity
// Useful for VERY common components
template <typename T>
class FlatComponentStorage : public IComponentStorage
{
public:
    virtual bool _has(Entity entity) const
    {
        uint32_t idx = entity.index;
        return m_mask.size() > idx && m_mask[idx] && T::at(idx) == entity;
    }

    virtual uint32_t _index(Entity entity) const
    {
        return entity.index;
    }

    virtual uint32_t _register(Entity entity, uint32_t currentSize)
    {
        (void)currentSize; // Not needed in flat component storage

        uint32_t idx = entity.index;
        if (m_mask.size() <= idx)
        {
            m_mask.resize(idx + 1);
        }
        m_mask[idx] = true;
        return idx;
    }

    virtual uint32_t _delete(Entity entity)
    {
        m_mask[entity.index] = false;
        return entity.index;
    }

    virtual uint32_t _safeDelete(Entity entity)
    {
        return _delete(entity);
    }

    virtual void
    _filter(bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
    {
        mask &= (filterType == IComponentStorage::WITH) ? m_mask : ~m_mask;
    }

    virtual void _clean()
    {
        EntityManager *em = EntityManager::getInstance();
        for (size_t i = 0; i < m_mask.size(); ++i)
        {
            if (m_mask[i] && !em->isAlive(T::at(i)))
            {
                m_mask[i] = false;
            }
        }
    }

private:
    bedrock::BitSet m_mask;
};

// Component storage using a map entity->index
class MapComponentStorage : public IComponentStorage
{
public:
    MapComponentStorage()
        : IComponentStorage()
        , m_maxEntityIndex(0u)
        , m_freeIndices()
        , m_indices()
    {}

    virtual bool _has(Entity entity) const
    {
        return m_indices.find(entity) != m_indices.end();
    }
    virtual uint32_t _index(Entity entity) const
    {
        return m_indices.find(entity)->second;
    }
    virtual uint32_t _register(Entity entity, uint32_t currentSize)
    {
        uint32_t eidx = entity.index;
        if (eidx > m_maxEntityIndex)
        {
            m_maxEntityIndex = eidx;
        }

        if (m_freeIndices.empty())
        {
            m_indices[entity] = currentSize;
            return currentSize;
        }
        else
        {
            uint32_t idx      = m_freeIndices.front();
            m_indices[entity] = idx;
            m_freeIndices.pop();
            return idx;
        }
    }

    virtual uint32_t _delete(Entity entity)
    {
        auto it      = m_indices.find(entity);
        uint32_t idx = it->second;
        m_freeIndices.push(idx);
        m_indices.erase(it);
        return idx;
    }

    virtual uint32_t _safeDelete(Entity entity)
    {
        return _delete(entity);
    }

    virtual void _filter(
        bedrock::BitSet &mask,
        IComponentStorage::ComponentStorageFilterType filterType) const
    {
        bedrock::BitSet localMask = bedrock::BitSet(
            m_maxEntityIndex + 1, filterType == IComponentStorage::WITHOUT);

        for (auto it : m_indices)
        {
            localMask[it.first.index] = (filterType == IComponentStorage::WITH);
        }

        mask &= localMask;
    }

    virtual void _clean()
    {
        EntityManager *em = EntityManager::getInstance();
        for (auto it = m_indices.cbegin(); it != m_indices.cend();)
        {
            if (!em->isAlive(it->first))
            {
                m_indices.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }

private:
    std::uint32_t m_maxEntityIndex;
    std::queue<uint32_t> m_freeIndices;
    std::unordered_map<Entity, uint32_t> m_indices;
};

typedef MapComponentStorage DefaultComponentStorage;
}} // namespace perfectpixel::ecs
