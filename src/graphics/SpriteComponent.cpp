#include <graphics/SpriteComponent.h>

namespace perfectpixel {
	namespace graphics {


SpriteComponent::SpriteComponent(
	world::Entity entity,
	graphics::Sprite *sprite,
	types::Vector2 worldSize,
	types::Vector2 offset,
	types::PpFloat fps,
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

perfectpixel::world::Entity SpriteComponent::getEntity() const
{
	return m_entity;
}

perfectpixel::graphics::Sprite * SpriteComponent::getSprite() const
{
	return m_sprite;
}

perfectpixel::types::PpInt SpriteComponent::getFrame() const
{
	return m_currentFrame;
}



perfectpixel::types::Vector2 SpriteComponent::getOffset() const
{
	return m_offset;
}

perfectpixel::types::Vector2 SpriteComponent::getSize() const
{
	return m_size;
}

perfectpixel::graphics::RenderHints SpriteComponent::getHints() const
{
	return m_renderHints;
}

void SpriteComponent::update(types::PpFloat deltaTime)
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
