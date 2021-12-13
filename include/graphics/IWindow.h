#pragma once

#include <Resources/Image.h>
#include <graphics/WindowSettings.h>

#include <Bedrock/InputCallback.h>

namespace perfectpixel { namespace graphics {

class IWindow
{
public:
    typedef void (*FocusCallbackFunc)(void *, bool);
    struct FocusCallback
    {
        FocusCallback()
            : m_func(nullptr)
            , m_instance(nullptr)
        {}

        FocusCallbackFunc m_func;
        void *m_instance;
    };

    typedef void (*SizeCallbackFunc)(void *, IWindow &, unsigned, unsigned);
    struct SizeCallback
    {
        SizeCallback()
            : m_func(nullptr)
            , m_instance(nullptr)
        {}

        SizeCallbackFunc m_func;
        void *m_instance;
    };

    IWindow() noexcept
    {}
    virtual ~IWindow()
    {}

public:
    virtual void initialize(const WindowSettings &settings) = 0;
    virtual void activate()                                 = 0;
    virtual void destroy()                                  = 0;
    virtual WindowDimensions getDimensions()                = 0;
    virtual bool isClosed()                                 = 0;
    virtual void draw()                                     = 0;
    virtual void setSplash(const resources::Image &image)   = 0;

    virtual void setKeyCallback(bedrock::KeyCallback callback) = 0;
    virtual void setFocusCallback(FocusCallback callback)      = 0;
    virtual void setResizeCallback(SizeCallback callback)      = 0;
};

}} // namespace perfectpixel::graphics
