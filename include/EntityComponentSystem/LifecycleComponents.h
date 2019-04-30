#pragma once

#include <EntityComponentSystem/Component.h>

namespace perfectpixel {
	namespace ecs {

		class ComponentRegistry;

		Component(CreationDoneLifecycleComponent) {};
		FinalizeComponent(CreationDoneLifecycleComponent)

		Component(DestroyedLifecycleComponent) {};
		FinalizeComponent(DestroyedLifecycleComponent)

		void registerLifecycleComponents(ComponentRegistry *registry);
} }
