#pragma once

#include "renderer/ImageResource.h"
#include "renderer/WindowSettings.h"

#include "bedrock/InputCallback.h"

namespace perfectpixel { namespace renderer {

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
    virtual void initialize(const WindowSettings &settings)            = 0;
    virtual void activate()                                            = 0;
    virtual void destroy()                                             = 0;
    virtual WindowDimensions getDimensions()                           = 0;
    virtual bool isClosed()                                            = 0;
    virtual void startFrame()                                          = 0;
    virtual void draw()                                                = 0;
    virtual void setSplash(int w, int h, int c, unsigned char *buffer) = 0;
    virtual void initImGui()                                           = 0;

    virtual void setKeyCallback(bedrock::KeyCallback callback) = 0;
    virtual void setFocusCallback(FocusCallback callback)      = 0;
    virtual void setResizeCallback(SizeCallback callback)      = 0;
};

}} // namespace perfectpixel::renderer
