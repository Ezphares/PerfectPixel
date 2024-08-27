#pragma once

#include "ecs/Component.h"

namespace perfectpixel { namespace ecs {

	class CreationDoneLifecycleComponent : public HintComponent<CreationDoneLifecycleComponent> {};
	class DestroyedLifecycleComponent : public HintComponent<DestroyedLifecycleComponent> {};

	class InactiveComponent : public HintComponent<InactiveComponent> {};
} }
