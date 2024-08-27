#pragma once

#include "renderer/BufferLayouts.h"
#include "renderer/CameraSettings.h"
#include "renderer/DrawQueue.h"
#include "renderer/FrameBuffer.h"
#include "renderer/IFont.h"
#include "renderer/LocalGL.h"
#include "renderer/Quad.h"
#include "renderer/RenderHint.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VAO.h"

#include "core/Resource.h"

#include "bedrock/vectors.h"

#include <map>

struct ImDrawData;

namespace perfectpixel { namespace renderer {
class RendererInterface
{
public:
    struct SpriteDrawInfo
    {
        Quad m_worldCoord, m_texCoord;
        RenderHints m_hints;
        Texture *m_texture;
        float m_depth;
    };

private:
    struct SpriteRenderState
    {
        GLuint m_texture;
        ShaderProgram *m_program;
    };

    typedef std::vector<SpriteDrawInfo> SpriteDrawList;

public:
    RendererInterface();
    ~RendererInterface();

public:
    void initialize();

    void startFrame();
    void drawAll(double deltaT);
    void postProcess();
    void endFrame(ImDrawData *);

    float calculateRatio(int32_t width, int32_t height);
    void setWindowRatio(float ratio);
    void setMainCamera(const CameraSettings &camera);
    void setWindowSize(bedrock::Point2 size);

    void queueDrawSingle(DrawQueueElement *element);

    IFont *getDefaultFont()
    {
        return m_font;
    }

    Texture &getImageTexture(core::Resource imageResource);
    void enqueueSpriteDraw(const SpriteDrawInfo &info);

private:
    void addSpriteToBuffer(const SpriteDrawInfo &info, SpriteBuffer *buffer);
    void addSpriteVertexToBuffer(
        const bedrock::Vector3 &pos,
        const bedrock::Vector2 &uv,
        SpriteBuffer *buffer);

    void drawSpriteList(
        const SpriteDrawList &list,
        SpriteRenderState *in_out_state,
        bool forceCurrentRenderstate = false);
    void flushSpriteBuffer(SpriteBuffer *buffer);

    bool isStateCompatible(
        const SpriteDrawInfo &info, const SpriteRenderState &state);
    void setCompatibleState(
        const SpriteDrawInfo &info, SpriteRenderState *out_state);

    void updateCamera();

    static bool compSortSoftalpha(
        const SpriteDrawInfo &first, const SpriteDrawInfo &second);

private:
    ShaderProgram *m_programSpriteHardAlpha;
    ShaderProgram *m_programSpriteSoftAlpha;
    ShaderProgram *m_programPostProcess;
    ShaderProgram *m_programUiText;

    FrameBuffer *m_frameBuffer;

    // GLuint m_ppbuffer;

    VAO *m_vaoDynamicSprites;
    VAO *m_vaoPostProcess;

    IFont *m_font;

    std::map<GLuint, SpriteDrawList> m_hardAlpha;
    SpriteDrawList m_softAlpha;

    bedrock::Point2 m_mainWindowSize;
    float m_windowRatio;
    CameraSettings m_mainCamera;

    std::vector<double> m_ftBuffer;
    std::size_t m_ftWrite;

    DrawQueue m_uiQueue;

    std::vector<Texture> m_managedTextures;
};

}} // namespace perfectpixel::renderer
