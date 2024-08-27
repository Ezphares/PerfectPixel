#include "core/SpriteDrawSystem.h"

#include "renderer/RendererInterface.h"

namespace perfectpixel::core {

renderer::RendererInterface *SpriteDrawSystem::m_gm = nullptr;

void SpriteDrawSystem::onRender(
    const RangeLimit &begin, const RangeLimit &end, float deltaT)
{
    for (auto it = begin; it != end; ++it)
    {
        auto entity = *it;

        renderer::SpriteComponent::Update(*it, static_cast<float>(deltaT));

        renderer::RendererInterface::SpriteDrawInfo drawInfo;

        bedrock::Vector3 actualPosition
            = bedrock::Vector3(renderer::SpriteComponent::Offset(entity));
        actualPosition
            += bedrock::Vector3(ecs::TransformComponent::Position(entity));
        bedrock::Vector2 upperBound = bedrock::Vector2(actualPosition)
                                      + renderer::SpriteComponent::Size(entity);

        drawInfo.m_worldCoord
            = {actualPosition.x, actualPosition.y, upperBound.x, upperBound.y};

        core::Sprite &sprite = renderer::SpriteComponent::SpriteData(entity);
        if (!sprite.getImage().isValid())
        {
            sprite = *renderer::SpriteComponent::SpriteResource(entity)
                          .get<core::Sprite>();
        }

        const bedrock::Vector2 texturePosition = sprite.getTexCoord(
            renderer::SpriteComponent::CurrentFrame(entity));
        const bedrock::Vector2 textureSize = sprite.getSize();

        drawInfo.m_texCoord
            = {texturePosition.x,
               texturePosition.y,
               texturePosition.x + textureSize.x,
               texturePosition.y + textureSize.y};

        drawInfo.m_hints
            = (renderer::RenderHints)renderer::SpriteComponent::DrawHint(
                entity);
        drawInfo.m_texture = &m_gm->getImageTexture(sprite.getImage());
        drawInfo.m_depth   = actualPosition.z;

        m_gm->enqueueSpriteDraw(drawInfo);
    }
}
} // namespace perfectpixel::core
