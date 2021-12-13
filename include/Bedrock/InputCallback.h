#pragma once

#include <cstdint>

namespace perfectpixel { namespace bedrock {

typedef std::uint32_t KeyCode;

enum KeyEvent
{
    PP_KEYUP,
    PP_KEYDOWN
};

typedef void (*KeyCallbackFunc)(void *, KeyCode, KeyEvent);
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
