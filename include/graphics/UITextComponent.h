#pragma once

#include <graphics/UIUtil.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <Bedrock/numbers.h>

#include <string>

namespace perfectpixel { namespace graphics {

	class UITextComponent
		: public ecs::Component<UITextComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		PPField(UITextComponent, std::string, Text);
		PPField(UITextComponent, int32_t, Alignment);
	};

} }
