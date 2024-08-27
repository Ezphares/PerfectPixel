#include "core/FileResourceLocator.h"

#include "bedrock/File.h"

#include <cstring>

namespace perfectpixel::core {

FileResourceLocator::FileResourceLocator()
    : m_LUT()
{}

bool FileResourceLocator::locate(
    int32_t type, int32_t id, char **out_buffer, size_t *out_size)
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
    int32_t type, int32_t id, const std::string &hint)
{
    m_LUT[key(type, id)] = hint;
}

int64_t FileResourceLocator::key(int32_t type, int32_t id)
{
    return (static_cast<uint64_t>(type) << 32) | static_cast<uint64_t>(id);
}

} // namespace perfectpixel::core
