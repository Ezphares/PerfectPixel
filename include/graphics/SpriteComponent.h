#pragma once

#include <graphics/RenderHint.h>

#include <EntityComponentSystem/Entity.h>

#include <Bedrock/vectors.h>

#include <Resources/Sprite.h>

namespace perfectpixel {
	namespace graphics {

		class SpriteComponent
		{
		public:
			SpriteComponent(
				ecs::Entity entity,
				resources::Sprite *m_sprite,

				bedrock::Vector2 worldSize,
				bedrock::Vector2 offset,
				float fps,

				RenderHints renderHints = RenderHints::RH_NONE
			);
			~SpriteComponent();

		public:
			ecs::Entity getEntity() const;

			resources::Sprite *getSprite() const;
			int32_t getFrame() const;

			bedrock::Vector2 getOffset() const;
			bedrock::Vector2 getSize() const;

			RenderHints getHints() const;

			void update(float deltaTime);

		private:
			ecs::Entity m_entity;
			resources::Sprite *m_sprite;
			bedrock::Vector2 m_size;
			bedrock::Vector2 m_offset;
			float m_fps;
			RenderHints m_renderHints;

			float m_frameTimeAccumulator;
			int32_t m_currentFrame;
		};
	}
}
