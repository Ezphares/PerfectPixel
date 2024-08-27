#include "ecs/LifeCycleSystem.h"

#include "ecs/LifecycleComponents.h"

namespace perfectpixel { namespace ecs {

namespace {
	void doKill(Entity entity)
	{
        EntityManager::getInstance()->kill(entity);
	}
}

void LifeCycleSystem::earlyAudit()
{
}

void LifeCycleSystem::lateAudit()
{
}

void LifeCycleSystem::init()
{
    CreationDoneLifecycleComponent::Mask()
        |= (EntityManager::getInstance()->all() & ~InactiveComponent::Mask());
}

void LifeCycleSystem::update(float deltaT)
{
    (void)deltaT;
}

void LifeCycleSystem::clean()
{
    bedrock::BitSet &mask  = DestroyedLifecycleComponent::Mask();

	EntityManager::getInstance()->expandMask(mask, nullptr, &doKill);

	mask &= bedrock::BitSet();
}

void LifeCycleSystem::render(float deltaT)
{
    (void)deltaT;
}

}}