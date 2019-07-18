#pragma once

#include <graphics/Sprite.h>
#include <graphics/RenderHint.h>

#include <EntityComponentSystem/Entity.h>
#include <Bedrock/vectors.h>

namespace perfectpixel {
	namespace graphics {

		class SpriteComponent
		{
		public:
			SpriteComponent(
				ecs::Entity entity,
				graphics::Sprite *m_sprite,

				bedrock::Vector2 worldSize,
				bedrock::Vector2 offset,
				bedrock::PpFloat fps,

				RenderHints renderHints = RenderHints::RH_NONE
			);
			~SpriteComponent();

		public:
			ecs::Entity getEntity() const;

			Sprite *getSprite() const;
			bedrock::PpInt getFrame() const;

			bedrock::Vector2 getOffset() const;
			bedrock::Vector2 getSize() const;

			RenderHints getHints() const;

			void update(bedrock::PpFloat deltaTime);

		private:
			ecs::Entity m_entity;
			graphics::Sprite *m_sprite;
			bedrock::Vector2 m_size;
			bedrock::Vector2 m_offset;
			bedrock::PpFloat m_fps;
			RenderHints m_renderHints;

			bedrock::PpFloat m_frameTimeAccumulator;
			bedrock::PpInt m_currentFrame;
		};
	}
}
