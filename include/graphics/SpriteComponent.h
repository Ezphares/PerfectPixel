#pragma once

#include <graphics/RenderHint.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <Bedrock/vectors.h>

#include <Resources/Sprite.h>
#include <Resources/Resource.h>

namespace perfectpixel {
	namespace graphics {

		class SpriteComponent
			: public ecs::Component<SpriteComponent>
			, public ecs::DefaultComponentStorage
		{
		public:
			PPField(SpriteComponent, resources::Sprite, SpriteData);
			PPResourceField(SpriteComponent, bedrock::typeID<resources::Sprite>(), SpriteResource);
			PPField(SpriteComponent, bedrock::Vector2, Size);
			PPField(SpriteComponent, bedrock::Vector2, Offset);
			PPField(SpriteComponent, float, FPS);
			PPField(SpriteComponent, uint32_t, DrawHint); // FIXME Renderhints Enum instead
			PPField(SpriteComponent, float, FrameTimeAccumulator);
			PPField(SpriteComponent, int32_t, CurrentFrame);

			static void Update(ecs::Entity entity, float deltaT);
		};
	}
}
