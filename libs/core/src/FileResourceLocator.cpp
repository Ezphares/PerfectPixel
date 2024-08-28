#include "core/FileResourceLocator.h"

#include "bedrock/File.h"

#include <cstring>

namespace perfectpixel::core {

FileResourceLocator::FileResourceLocator()
    : m_LUT()
{}

bool FileResourceLocator::locate(
    bedrock::TypeID type, bedrock::ID id, char **out_buffer, size_t *out_size)
{
    auto it = m_LUT.find(key(type, id));
    if (it == m_LUT.end())
    {
        *out_buffer = nullptr;
        *out_size   = 0;
        return false;
    }

    bedrock::File file = bedrock::File(it->second);

    *out_size   = file.getSize();
    *out_buffer = new char[file.getSize()];
    memcpy(*out_buffer, file.getBuffer(), *out_size);

    return true;
}

void FileResourceLocator::insert(
    bedrock::TypeID type, bedrock::ID id, const std::string &hint)
{
    m_LUT[key(type, id)] = hint;
}

int64_t FileResourceLocator::key(bedrock::TypeID type, bedrock::ID id)
{
    return (static_cast<uint64_t>(type) << 32)
           | static_cast<uint64_t>(id.m_hash);
}

} // namespace perfectpixel::core
