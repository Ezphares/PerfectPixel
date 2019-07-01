#pragma once

#include <graphics/UIUtil.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <types/numbers.h>

#include <string>

namespace perfectpixel { namespace graphics {

	class UITextComponent
		: public ecs::Component<UITextComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		inline static ecs::Field<UITextComponent, std::string> Text;
		inline static ecs::Field<UITextComponent, types::PpInt> Alignment;
	};

} }
