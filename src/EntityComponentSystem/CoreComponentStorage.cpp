#include <EntityComponentSystem/CoreComponentStorage.h>

#include <EntityComponentSystem/EntityManager.h>

namespace perfectpixel { namespace ecs {
/************************************************************************/
/*                                                                      */
/************************************************************************/
LinearScanComponentStorage::LinearScanComponentStorage()
    : m_maxEntityIndex(0u)
    , m_entities()
{}

bool LinearScanComponentStorage::_has(Entity entity) const
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

uint32_t LinearScanComponentStorage::_index(Entity entity) const
{
    for (uint32_t i = 0u; i < m_entities.size(); ++i)
    {
        if (m_entities[i] == entity)
        {
            return i;
        }
    }
    return ~0u;
}

perfectpixel::ecs::Entity LinearScanComponentStorage::_at(uint32_t index) const
{
    return m_entities[index];
}

uint32_t
LinearScanComponentStorage::_register(Entity entity, uint32_t currentSize)
{
    (void)currentSize; // Not needed in LinearScan

    uint32_t eidx = entity.index;
    if (eidx > m_maxEntityIndex)
    {
        m_maxEntityIndex = eidx;
    }

    uint32_t idx = (uint32_t)m_entities.size();
    m_entities.push_back(entity);
    return idx;
}

uint32_t LinearScanComponentStorage::_delete(Entity entity)
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

uint32_t LinearScanComponentStorage::_safeDelete(Entity entity)
{
    return _delete(entity);
}

void LinearScanComponentStorage::_filter(
    bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
{
    bedrock::BitSet localMask = bedrock::BitSet(
        m_maxEntityIndex + 1,
        filterType == ComponentStorageFilterType::WITHOUT);
    for (auto entity : m_entities)
    {
        localMask[entity.index]
            = (filterType == ComponentStorageFilterType::WITH);
    }

    mask &= localMask;
}

void LinearScanComponentStorage::_clean()
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
/************************************************************************/
/*                                                                      */
/************************************************************************/
FlatComponentStorage::FlatComponentStorage()
    : m_mask()
    , m_entities()
{}

bool FlatComponentStorage::_has(Entity entity) const
{
    uint32_t idx = entity.index;
    return m_mask.size() > idx && m_mask[idx] && _at(idx) == entity;
}

uint32_t FlatComponentStorage::_index(Entity entity) const
{
    return entity.index;
}

perfectpixel::ecs::Entity FlatComponentStorage::_at(uint32_t index) const
{
    return m_entities[index];
}

uint32_t FlatComponentStorage::_register(Entity entity, uint32_t currentSize)
{
    (void)currentSize; // Not needed in flat component storage

    uint32_t idx = entity.index;
    if (m_mask.size() <= idx)
    {
        m_mask.resize((idx + 1));
    }
    if (m_entities.size() <= idx)
    {
        m_entities.resize((idx + 1), NO_ENTITY);
    }
    m_mask[idx] = true;
    return idx;
}

uint32_t FlatComponentStorage::_delete(Entity entity)
{
    m_mask[entity.index]     = false;
    m_entities[entity.index] = NO_ENTITY;
    return entity.index;
}

uint32_t FlatComponentStorage::_safeDelete(Entity entity)
{
    return _delete(entity);
}

void FlatComponentStorage::_filter(
    bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
{
    mask &= (filterType == ComponentStorageFilterType::WITH) ? m_mask : ~m_mask;
}

void FlatComponentStorage::_clean()
{
    EntityManager *em = EntityManager::getInstance();
    for (uint32_t i = 0; i < static_cast<uint32_t>(m_mask.size()); ++i)
    {
        if (m_mask[i] && !em->isAlive(_at(i)))
        {
            m_mask[i] = false;
        }
    }
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
MapComponentStorage::MapComponentStorage()
    : m_maxEntityIndex(0u)
    , m_freeIndices()
    , m_indices()
{}

bool MapComponentStorage::_has(Entity entity) const
{
    return m_indices.find(entity) != m_indices.end();
}

uint32_t MapComponentStorage::_index(Entity entity) const
{
    return m_indices.find(entity)->second;
}

perfectpixel::ecs::Entity MapComponentStorage::_at(uint32_t index) const
{
    for (auto it : m_indices)
    {
        if (it.second == index)
        {
            return it.first;
        }
    }

    return NO_ENTITY;
}

uint32_t MapComponentStorage::_register(Entity entity, uint32_t currentSize)
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

uint32_t MapComponentStorage::_delete(Entity entity)
{
    auto it      = m_indices.find(entity);
    uint32_t idx = it->second;
    m_freeIndices.push(idx);
    m_indices.erase(it);
    return idx;
}

uint32_t MapComponentStorage::_safeDelete(Entity entity)
{
    return _delete(entity);
}

void MapComponentStorage::_filter(
    bedrock::BitSet &mask, ComponentStorageFilterType filterType) const
{
    bedrock::BitSet localMask = bedrock::BitSet(
        m_maxEntityIndex + 1,
        filterType == ComponentStorageFilterType::WITHOUT);

    for (auto it : m_indices)
    {
        localMask[it.first.index]
            = (filterType == ComponentStorageFilterType::WITH);
    }

    mask &= localMask;
}

void MapComponentStorage::_clean()
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

}} // namespace perfectpixel::ecs