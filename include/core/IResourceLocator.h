#pragma once

#include "bedrock/Reflect.h"

#include <cstdint>
#include <string>

namespace perfectpixel::core {

class IResourceLocator
{
public:
    virtual bool locate(
        bedrock::TypeID type,
        bedrock::ID id,
        char **out_buffer,
        size_t *out_size)
        = 0;
    virtual void
    insert(bedrock::TypeID type, bedrock::ID id, const std::string &hint)
        = 0;
};

} // namespace perfectpixel::core
