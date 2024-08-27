#pragma once

#include "core/IResourceLocator.h"

#include <map>

namespace perfectpixel::core {

class FileResourceLocator : public IResourceLocator
{
public:
    FileResourceLocator();

    virtual bool locate(
        int32_t type, int32_t id, char **out_buffer, size_t *out_size) override;
    virtual void
    insert(int32_t type, int32_t id, const std::string &hint) override;

private:
    int64_t key(int32_t type, int32_t id);

private:
    // TODO: Deal with storage, or just make a binary locator and let this one
    // suck?
    std::map<int64_t, std::string> m_LUT;
};

} // namespace perfectpixel::core
