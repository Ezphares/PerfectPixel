#include "core/SpriteDrawSystem.h"

#include "renderer/RendererInterface.h"

namespace perfectpixel::core {

renderer::RendererInterface *SpriteDrawSystem::m_gm = nullptr;

void SpriteDrawSystem::onRender(std::span<ecs::Entity> entities, float deltaT)
{
    for (auto entity : entities)
    {
        SpriteComponent::Update(entity, static_cast<float>(deltaT));

        renderer::RendererInterface::SpriteDrawInfo drawInfo;

        bedrock::Vector3 actualPosition
            = bedrock::Vector3(SpriteComponent::Offset(entity));
        actualPosition
            += bedrock::Vector3(ecs::TransformComponent::Position(entity));
        bedrock::Vector2 upperBound
            = bedrock::Vector2(actualPosition) + SpriteComponent::Size(entity);

        drawInfo.m_worldCoord
            = {actualPosition.x, actualPosition.y, upperBound.x, upperBound.y};

        core::Sprite &sprite = SpriteComponent::SpriteData(entity);
        if (!sprite.getImage().isValid())
        {
            sprite
                = *SpriteComponent::SpriteResource(entity).get<core::Sprite>();
        }

        const bedrock::Vector2 texturePosition
            = sprite.getTexCoord(SpriteComponent::CurrentFrame(entity));
        const bedrock::Vector2 textureSize = sprite.getSize();

        drawInfo.m_texCoord
            = {texturePosition.x,
               texturePosition.y,
               texturePosition.x + textureSize.x,
               texturePosition.y + textureSize.y};

        drawInfo.m_hints
            = (renderer::RenderHints)SpriteComponent::DrawHint(entity);

        Resource imageResource = sprite.getImage();

        constexpr renderer::ImageResourceBundleID globalBundle = ~0;
        renderer::ImageResourceBundleID bundleID               = globalBundle;
        const bedrock::UniqueVoidPtr &userData = imageResource.getUserData();
        if (userData)
        {
            bundleID = userData.get<renderer::ImageResourceUserData>().bundleID;
        }
        drawInfo.m_texture = &m_gm->getImageTexture(
            imageResource.getId(), bundleID, &getImageResource, &imageResource);
        drawInfo.m_depth = actualPosition.z;

        m_gm->enqueueSpriteDraw(drawInfo);
    }
}

renderer::ImageResource *SpriteDrawSystem::getImageResource(void *resourcePtr)
{
    return reinterpret_cast<Resource *>(resourcePtr)
        ->get<renderer::ImageResource>();
}

} // namespace perfectpixel::core
