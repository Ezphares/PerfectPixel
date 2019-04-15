#pragma once

#include <graphics/Sprite.h>
#include <graphics/RenderHint.h>

#include <EntityComponentSystem/Entity.h>

#include <types/vectors.h>

namespace perfectpixel {
	namespace graphics {

		class SpriteComponent
		{
		public:
			SpriteComponent(
				ecs::Entity entity,
				graphics::Sprite *m_sprite,

				types::Vector2 worldSize,
				types::Vector2 offset,
				types::PpFloat fps,

				RenderHints renderHints = RenderHints::RH_NONE
			);
			~SpriteComponent();

		public:
			ecs::Entity getEntity() const;

			Sprite *getSprite() const;
			types::PpInt getFrame() const;

			types::Vector2 getOffset() const;
			types::Vector2 getSize() const;

			RenderHints getHints() const;

			void update(types::PpFloat deltaTime);

		private:
			ecs::Entity m_entity;
			graphics::Sprite *m_sprite;
			types::Vector2 m_size;
			types::Vector2 m_offset;
			types::PpFloat m_fps;
			RenderHints m_renderHints;

			types::PpFloat m_frameTimeAccumulator;
			types::PpInt m_currentFrame;
		};
	}
}
