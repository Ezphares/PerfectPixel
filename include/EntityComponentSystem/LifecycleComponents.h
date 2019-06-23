#pragma once

#include <EntityComponentSystem/Component.h>

namespace perfectpixel { namespace ecs {

	class CreationDoneLifecycleComponent : public HintComponent<CreationDoneLifecycleComponent> {};
	class DestroyedLifecycleComponent : public HintComponent<DestroyedLifecycleComponent> {};
} }
