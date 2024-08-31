#pragma once

#include "bedrock/Reflect.h"

namespace perfectpixel { namespace ecs {

struct FastUpdate
{
    int32_t m_id;

    template <typename T>
    static FastUpdate Build()
    {
        return FastUpdate{bedrock::Reflect<T>::id()};
    }
};

}} // namespace perfectpixel::ecs
