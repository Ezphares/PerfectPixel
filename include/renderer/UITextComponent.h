#pragma once

#include "renderer/UIUtil.h"

#include "ecs/Component.h"
#include "ecs/CoreComponentStorage.h"

#include "bedrock/numbers.h"

#include <string>

namespace perfectpixel { namespace renderer {

class UITextComponent
    : public ecs::Component<UITextComponent, ecs::LinearScanComponentStorage>
{
public:
    PPField(UITextComponent, std::string, Text);
    PPField(UITextComponent, int32_t, Alignment);
};

}} // namespace perfectpixel::renderer
