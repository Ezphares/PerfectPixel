#pragma once

#include "core/IResourceLocator.h"

#include <map>

#include "bedrock/Reflect.h"

namespace perfectpixel::core {

class FileResourceLocator : public IResourceLocator
{
public:
    FileResourceLocator();

    virtual bool locate(
        bedrock::TypeID type,
        bedrock::ID id,
        char **out_buffer,
        size_t *out_size) override;
    virtual void insert(
        bedrock::TypeID type, bedrock::ID id, const std::string &hint) override;

private:
    int64_t key(bedrock::TypeID type, bedrock::ID id);

private:
    // TODO: Deal with storage, or just make a binary locator and let this one
    // suck?
    std::map<int64_t, std::string> m_LUT;
};

} // namespace perfectpixel::core
