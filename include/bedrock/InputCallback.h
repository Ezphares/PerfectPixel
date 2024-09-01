#pragma once

#include <cstdint>

#include "InputEnums.h"

namespace perfectpixel { namespace bedrock {

enum KeyEvent
{
    PP_KEYUP,
    PP_KEYDOWN
};

typedef void (*KeyCallbackFunc)(void *, KeyboardButton, KeyEvent);
struct KeyCallback
{
    KeyCallback()
        : m_func(nullptr)
        , m_instance(nullptr)
    {}

    KeyCallbackFunc m_func;
    void *m_instance;
};

}} // namespace perfectpixel::bedrock
