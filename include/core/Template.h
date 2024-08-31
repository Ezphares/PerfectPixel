#pragma once

#include "core/Resource.h"
#include "core/ResourceManager.h"

#include "ecs/Entity.h"

namespace perfectpixel::core {

class Template
{
public:
    Template();

    ecs::Entity spawn();

    void applyTo(std::vector<ecs::Entity> &target);

    static ResourceManager::ResourceLoaderFunction CreateTemplateLoader();
    static void TemplateUnloader(void **data);

private:
    void prepare(std::vector<ecs::Entity> &all);
    void spawnPrepared();

    struct Preparation
    {
        uint32_t m_start;
        std::vector<uint32_t> m_idxMap;
    };

    std::vector<ecs::Entity> m_entities;
    std::vector<Resource> m_subTemplates;
};

} // namespace perfectpixel::core

PP_REFLECT(::perfectpixel::core::Template);
