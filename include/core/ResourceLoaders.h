#pragma once

#include "bedrock/UniqueVoidPtr.h"

namespace perfectpixel::core {
void imageLoaderFunc(
    char *data,
    size_t dataSize,
    void **target,
    const bedrock::UniqueVoidPtr &userData,
    void *loaderUserData);
void imageUnloaderFunc(void **target);
} // namespace perfectpixel::core