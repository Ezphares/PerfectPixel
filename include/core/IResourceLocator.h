#pragma once

#include <cstdint>
#include <string>

namespace perfectpixel::core {

class IResourceLocator
{
public:
    virtual bool
    locate(int32_t type, int32_t id, char **out_buffer, size_t *out_size)
        = 0;
    virtual void insert(int32_t type, int32_t id, const std::string &hint) = 0;
};

} // namespace perfectpixel::core
