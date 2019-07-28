#include <graphics/SpriteComponent.h>

namespace perfectpixel {
	namespace graphics {


SpriteComponent::SpriteComponent(
	ecs::Entity entity,
	resources::Sprite *sprite,
	bedrock::Vector2 worldSize,
	bedrock::Vector2 offset,
	float fps,
	RenderHints renderHints)
	: m_entity(entity)
	, m_sprite(sprite)
	, m_size(worldSize)
	, m_offset(offset)
	, m_fps(fps)
	, m_renderHints(renderHints)
	, m_frameTimeAccumulator(0)
	, m_currentFrame(0)
{

}

SpriteComponent::~SpriteComponent()
{
}

perfectpixel::ecs::Entity SpriteComponent::getEntity() const
{
	return m_entity;
}

perfectpixel::resources::Sprite * SpriteComponent::getSprite() const
{
	return m_sprite;
}

int32_t SpriteComponent::getFrame() const
{
	return m_currentFrame;
}



perfectpixel::bedrock::Vector2 SpriteComponent::getOffset() const
{
	return m_offset;
}

perfectpixel::bedrock::Vector2 SpriteComponent::getSize() const
{
	return m_size;
}

perfectpixel::graphics::RenderHints SpriteComponent::getHints() const
{
	return m_renderHints;
}

void SpriteComponent::update(float deltaTime)
{
	m_frameTimeAccumulator += deltaTime;
	if (m_frameTimeAccumulator > 1 / m_fps)
	{
		m_frameTimeAccumulator -= 1 / m_fps;
		m_currentFrame++;
	}
}

	}
}
