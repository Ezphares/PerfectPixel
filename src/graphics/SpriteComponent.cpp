#include <graphics/SpriteComponent.h>

namespace perfectpixel {
	namespace graphics {

void SpriteComponent::SetSprite(ecs::Entity entity, resources::Resource &res)
{
	resources::Resource &spr = SpriteComponent::SpriteResource(entity);
	// if (spr != res) TODO: Implement equality operators
	{
		spr = res;
		SpriteComponent::SpriteData(entity) = *spr.get<resources::Sprite>();
	}
}

void SpriteComponent::Update(ecs::Entity entity, float deltaT)
{
	float &fta = FrameTimeAccumulator(entity);
	float frameTime = 1.0f / FPS(entity);
	fta += deltaT;
	if (fta > frameTime)
	{
		fta -= frameTime;
		CurrentFrame(entity)++;
	}
}

	}
}
