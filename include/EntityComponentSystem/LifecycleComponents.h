#pragma once

#include <EntityComponentSystem/Component.h>

namespace perfectpixel {
	namespace world {

		class ComponentRegistry;

		Component(CreatedLifecycleComponent) {};
		FinalizeComponent(CreatedLifecycleComponent)

		Component(DestroyedLifecycleComponent) {};
		FinalizeComponent(DestroyedLifecycleComponent)

		static void registerLifecycleComponents(ComponentRegistry *registry);
} }
