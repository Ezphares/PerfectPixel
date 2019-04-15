
#include <worldgraph/LifecycleComponents.h>
#include <worldgraph/ComponentRegistry.h>
#include <worldgraph/HintComponentContainer.h>

namespace perfectpixel {namespace world {

	void registerLifecycleComponents(ComponentRegistry *registry)
	{
		registry->registerStorage(CreatedLifecycleComponent::getTypeId(), new HintComponentContainer<CreatedLifecycleComponent>(), true);
		registry->registerStorage(DestroyedLifecycleComponent::getTypeId(), new HintComponentContainer<DestroyedLifecycleComponent>(), true);
	}

} }
