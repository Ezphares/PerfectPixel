#include <graphics/SpriteComponent.h>

namespace perfectpixel {
	namespace graphics {

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
