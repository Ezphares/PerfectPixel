#pragma once

#include "bedrock/Opaque.h"

namespace perfectpixel::core {
void imageLoaderFunc(
    char *data,
    size_t dataSize,
    void **target,
    const bedrock::Opaque &userData,
    void *loaderUserData);
void imageUnloaderFunc(void **target);
} // namespace perfectpixel::core