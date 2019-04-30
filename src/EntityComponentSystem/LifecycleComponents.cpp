
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/ComponentRegistry.h>
#include <EntityComponentSystem/HintComponentContainer.h>

namespace perfectpixel {namespace ecs {

	void perfectpixel::ecs::registerLifecycleComponents(ComponentRegistry *registry)
	{
		registry->registerStorage(CreationDoneLifecycleComponent::getTypeId(), new HintComponentContainer<CreationDoneLifecycleComponent>(), true);
		registry->registerStorage(DestroyedLifecycleComponent::getTypeId(), new HintComponentContainer<DestroyedLifecycleComponent>(), true);
	}

} }
