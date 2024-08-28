#include "renderer/RendererInterface.h"

#include "renderer/CBFGFont.h"
#include "renderer/CameraComponent.h"
#include "renderer/LocalGL.h"

#include "bedrock/File.h"
#include "bedrock/PpException.h"

#include <fstream>
#include <iomanip>
#include <sstream>

// clang-format off
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
// clang-format on

namespace perfectpixel { namespace renderer {

namespace {
// clang-format off
const static GLfloat MAT4_IDENTITY[16]{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1};
// clang-format on

// clang-format off
const static GLfloat PP_VERTICES[18]{
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
     1.0f,  1.0f,  0.0f
};
// clang-format on

const static GLfloat COLOUR_WHITE[4]{1, 1, 1, 1};
} // namespace

RendererInterface::RendererInterface()
    : m_programSpriteHardAlpha(NULL)
    , m_programSpriteSoftAlpha(NULL)
    , m_programPostProcess(NULL)
    , m_windowRatio(1.0f)
    , m_mainCamera()
    , m_ftWrite(0)
    , m_textureStore()
{}

RendererInterface::~RendererInterface()
{}

void RendererInterface::initialize()
{

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glewInit();

    // DEBUG
#if _WIN32
    wglewInit();
    wglSwapIntervalEXT(0);
#endif
    // /DEBUG

    ImGui_ImplOpenGL3_Init();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VAO::BufferLayout spriteLayout;
    spriteLayout.push_back(VAO::BE_VEC_3); // Xyz
    spriteLayout.push_back(VAO::BE_VEC_2); // Uv

    m_vaoDynamicSprites = new VAO(spriteLayout);

    VAO::BufferLayout ppLayout;
    ppLayout.push_back(VAO::BE_VEC_3);
    m_vaoPostProcess = new VAO(ppLayout);

    std::string vertex_shader
        = bedrock::File("Sprite_vertex.glsl").str(),
        fragment_shader_hard = bedrock::File("SpriteHard_fragment.glsl").str(),
        fragment_shader_soft = bedrock::File("SpriteSoft_fragment.glsl").str(),
        vertex_shader_pp     = bedrock::File("PostProc_vertex.glsl").str(),
        fragment_shader_pp   = bedrock::File("PostProc_fragment.glsl").str(),
        fragment_shader_text = bedrock::File("Text_fragment.glsl").str();

    m_programSpriteHardAlpha = new ShaderProgram();
    m_programSpriteHardAlpha->addShader(GL_VERTEX_SHADER, vertex_shader);
    m_programSpriteHardAlpha->addShader(
        GL_FRAGMENT_SHADER, fragment_shader_hard);
    m_programSpriteHardAlpha->link();
    m_programSpriteHardAlpha->use();
    glUniform1i(m_programSpriteHardAlpha->getUniformLocation("u_texture"), 1);
    glUniformMatrix4fv(
        m_programSpriteHardAlpha->getUniformLocation("u_transform"),
        1,
        GL_FALSE,
        MAT4_IDENTITY);

    m_programSpriteSoftAlpha = new ShaderProgram();
    m_programSpriteSoftAlpha->addShader(GL_VERTEX_SHADER, vertex_shader);
    m_programSpriteSoftAlpha->addShader(
        GL_FRAGMENT_SHADER, fragment_shader_soft);
    m_programSpriteSoftAlpha->link();
    m_programSpriteSoftAlpha->use();
    glUniform1i(m_programSpriteSoftAlpha->getUniformLocation("u_texture"), 1);
    glUniformMatrix4fv(
        m_programSpriteSoftAlpha->getUniformLocation("u_transform"),
        1,
        GL_FALSE,
        MAT4_IDENTITY);

    m_programPostProcess = new ShaderProgram();
    m_programPostProcess->addShader(GL_VERTEX_SHADER, vertex_shader_pp);
    m_programPostProcess->addShader(GL_FRAGMENT_SHADER, fragment_shader_pp);
    m_programPostProcess->link();
    m_programPostProcess->use();
    glUniform1i(m_programPostProcess->getUniformLocation("u_texture"), 1);

    m_programUiText = new ShaderProgram();
    m_programUiText->addShader(GL_VERTEX_SHADER, vertex_shader);
    m_programUiText->addShader(GL_FRAGMENT_SHADER, fragment_shader_text);
    m_programUiText->link();
    m_programUiText->use();
    glUniform1i(m_programUiText->getUniformLocation("u_texture"), 1);
    glUniform4fv(
        m_programUiText->getUniformLocation("u_fontColour"), 1, COLOUR_WHITE);

    m_frameBuffer = new FrameBuffer({0, 0});

    m_vaoPostProcess->bindBuffer();
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(PP_VERTICES), PP_VERTICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_font = new CBFGFont(bedrock::File("DengXianLight.bff").str());
}

void RendererInterface::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
}

void RendererInterface::startFrame()
{
    ImGui_ImplOpenGL3_NewFrame();

    m_softAlpha.clear();
    m_hardAlpha.clear();
}

void RendererInterface::drawAll(double deltaT)
{
    (void)deltaT;

    m_frameBuffer->bind();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::sort(
        m_softAlpha.begin(),
        m_softAlpha.end(),
        &RendererInterface::compSortSoftalpha);

    SpriteRenderState renderState{0, 0};
    for (auto it : m_hardAlpha)
    {
        setCompatibleState(it.second[0], &renderState);
        drawSpriteList(it.second, &renderState, true);
    }

    drawSpriteList(m_softAlpha, &renderState);

    postProcess();

    TextBuffer text;
    m_font->getTexture().bind();

    // FIXME Decent ui pass
    for (DrawQueueElement *element : m_uiQueue)
    {
        switch (element->m_type)
        {
        case (DQET_UI_TEXT): {
            UITextDrawQueueElement *textElement
                = static_cast<UITextDrawQueueElement *>(element);

            // FIXME default font for now
            if (!textElement->m_font)
            {
                textElement->m_font = m_font;
            }

            textElement->m_font->writeBuffer(
                textElement->m_position,
                textElement->m_fontSize,
                textElement->m_text,
                &text);
        }
        break;
        default:
            // TODO
            break;
        }

        DrawQueueElementFactory::getInstance()->deallocate(element);
    }
    m_uiQueue.clear();

    m_programUiText->use();

    m_vaoDynamicSprites->bindVAO();
    m_vaoDynamicSprites->bindBuffer();

    glBufferData(
        GL_ARRAY_BUFFER,
        text.size() * sizeof(TextVertex),
        text.data(),
        GL_STREAM_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(text.size()));
}

void RendererInterface::postProcess()
{
    //
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (m_mainCamera.m_scaleMode == CameraSettings::SCALE_BARS)
    {
        const float cameraRatio = m_mainCamera.m_size.x / m_mainCamera.m_size.y;
        if (m_windowRatio > cameraRatio)
        {
            scaleX *= (m_windowRatio / cameraRatio);
        }
        else
        {
            scaleY *= (cameraRatio / m_windowRatio);
        }
    }
    //

    // Draw to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(
        (int)(m_mainWindowSize.m_x - m_mainWindowSize.m_x / scaleX) / 2,
        (int)(m_mainWindowSize.m_y - m_mainWindowSize.m_y / scaleY) / 2,
        (int)(m_mainWindowSize.m_x / scaleX),
        (int)(m_mainWindowSize.m_y / scaleY));
    glClearColor(0, 0.5f, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use post processing program
    m_programPostProcess->use();

    // Use the texture drawn from the main processing cycle
    glActiveTexture(GL_TEXTURE0 + 1);
    m_frameBuffer->getTexture().bind();

    // Activate the VAO buffer
    m_vaoPostProcess->bindVAO();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RendererInterface::endFrame(ImDrawData *imgui)
{
    ImGui_ImplOpenGL3_RenderDrawData(imgui);
}

float RendererInterface::calculateRatio(int32_t width, int32_t height)
{
    return static_cast<float>(width) / static_cast<float>(height);
}

void RendererInterface::setWindowRatio(float ratio)
{
    m_windowRatio = ratio;
    updateCamera();
}

void RendererInterface::setMainCamera(const CameraSettings &camera)
{
    m_mainCamera = camera;
    updateCamera();
}

void RendererInterface::setWindowSize(bedrock::Point2 size)
{
    m_mainWindowSize = size;
    setWindowRatio(calculateRatio(size.m_x, size.m_y));
}

void RendererInterface::queueDrawSingle(DrawQueueElement *element)
{
    switch (element->m_type)
    {
    case (DQET_UI_TEXT):
        m_uiQueue.push_back(element);
        break;

    case (DQET_SPRITE):
        // m_spriteQueue.push_back(element);
        break;

    default:
        break;
    }
}

void RendererInterface::enqueueSpriteDraw(const SpriteDrawInfo &info)
{
    if (info.m_hints & RenderHints::RH_SOFTALPHA)
    {
        m_softAlpha.push_back(info);
    }
    else
    {
        m_hardAlpha[info.m_texture->getId()].push_back(info);
    }
}

void RendererInterface::addSpriteToBuffer(
    const SpriteDrawInfo &info, SpriteBuffer *buffer)
{
    const Quad &pos{info.m_worldCoord}, &tex{info.m_texCoord};

    float z{info.m_depth};

    bedrock::Vector3 posTopLeft{pos.m_left, pos.m_top, z},
        posTopRight{pos.m_right, pos.m_top, z},
        posBottomLeft{pos.m_left, pos.m_bottom, z},
        posBottomRight{pos.m_right, pos.m_bottom, z};

    const bedrock::Vector2 texTopLeft{tex.m_left, tex.m_top},
        texTopRight{tex.m_right, tex.m_top},
        texBottomLeft{tex.m_left, tex.m_bottom},
        texBottomRight{tex.m_right, tex.m_bottom};

    // First tri
    addSpriteVertexToBuffer(posTopLeft, texTopLeft, buffer);
    addSpriteVertexToBuffer(posTopRight, texTopRight, buffer);
    addSpriteVertexToBuffer(posBottomLeft, texBottomLeft, buffer);

    // Second tri
    addSpriteVertexToBuffer(posBottomLeft, texBottomLeft, buffer);
    addSpriteVertexToBuffer(posTopRight, texTopRight, buffer);
    addSpriteVertexToBuffer(posBottomRight, texBottomRight, buffer);
}

void RendererInterface::addSpriteVertexToBuffer(
    const bedrock::Vector3 &pos,
    const bedrock::Vector2 &uv,
    SpriteBuffer *buffer)
{
    SpriteVertex vertex{
        static_cast<GLfloat>(pos.x),
        static_cast<GLfloat>(pos.y),
        static_cast<GLfloat>(pos.z),

        static_cast<GLfloat>(uv.x),
        static_cast<GLfloat>(uv.y)};

    buffer->push_back(vertex);
}

void RendererInterface::drawSpriteList(
    const SpriteDrawList &list,
    SpriteRenderState *in_out_state,
    bool forceCurrentState)
{
    SpriteBuffer buffer;
    for (auto &info : list)
    {
        if (!forceCurrentState && !isStateCompatible(info, *in_out_state))
        {
            flushSpriteBuffer(&buffer);

            setCompatibleState(info, in_out_state);
        }

        addSpriteToBuffer(info, &buffer);
    }

    flushSpriteBuffer(&buffer);
}

void RendererInterface::flushSpriteBuffer(SpriteBuffer *buffer)
{
    if (!buffer->empty())
    {
        m_vaoDynamicSprites->bindBuffer();
        glBufferData(
            GL_ARRAY_BUFFER,
            buffer->size() * sizeof(SpriteVertex),
            buffer->data(),
            GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_vaoDynamicSprites->bindVAO();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(buffer->size()));
        glBindVertexArray(0);
    }

    buffer->clear();
}

bool RendererInterface::isStateCompatible(
    const SpriteDrawInfo &info, const SpriteRenderState &state)
{
    if (state.m_texture != info.m_texture->getId())
    {
        return false;
    }

    ShaderProgram *targetProgram = info.m_hints & RH_SOFTALPHA
                                       ? m_programSpriteSoftAlpha
                                       : m_programSpriteHardAlpha;
    return targetProgram == state.m_program;
}

void RendererInterface::setCompatibleState(
    const SpriteDrawInfo &info, SpriteRenderState *out_state)
{
    // Figure out program depending on alpha settings
    ShaderProgram *targetProgram = info.m_hints & RH_SOFTALPHA
                                       ? m_programSpriteSoftAlpha
                                       : m_programSpriteHardAlpha;
    if (out_state->m_program != targetProgram)
    {
        targetProgram->use();
        out_state->m_program = targetProgram;
    }

    // Bind desired texture
    GLuint targetTexture = info.m_texture->getId();
    if (out_state->m_texture != targetTexture)
    {
        info.m_texture->bind(1);
        out_state->m_texture = targetTexture;
    }
}

void RendererInterface::updateCamera()
{
    float cameraRatio = m_mainCamera.m_size.x / m_mainCamera.m_size.y;

    bedrock::Vector2 scale = m_mainCamera.m_size;

    if (std::abs(m_windowRatio - cameraRatio) > 0.001)
    {
        // FIXME update for non-stretch modes
    }

    bedrock::Vector2 translate{
        m_mainCamera.m_center.x / scale.x, m_mainCamera.m_center.y / scale.y};

    // clang-format off
    GLfloat cameraTransform[16]{
        2 / scale.x,		0,					0, 0, 
		0,					2 / scale.y,		0, 0,
        0,					0,					1, 0,
        -translate.x * 2, -translate.y * 2, 0, 1};
    // clang-format on

    m_programSpriteSoftAlpha->use();
    glUniformMatrix4fv(
        m_programSpriteSoftAlpha->getUniformLocation("u_transform"),
        1,
        GL_FALSE,
        cameraTransform);

    m_programSpriteHardAlpha->use();
    glUniformMatrix4fv(
        m_programSpriteHardAlpha->getUniformLocation("u_transform"),
        1,
        GL_FALSE,
        cameraTransform);

    m_programUiText->use();
    glUniformMatrix4fv(
        m_programUiText->getUniformLocation("u_transform"),
        1,
        GL_FALSE,
        cameraTransform);

    m_frameBuffer->resize(
        {static_cast<int32_t>(scale.x), static_cast<int32_t>(scale.y)});
}

const perfectpixel::renderer::Texture &RendererInterface::getImageTexture(
    bedrock::ID resourceID,
    ImageResourceBundleID bundleID,
    ImageResource *(*getResource)(void *),
    void *resourcePtr)
{
    const Texture *tex = m_textureStore.getTexture(bundleID, resourceID);
    if (tex)
        return *tex;

    return m_textureStore.createTexture(
        bundleID, resourceID, *getResource(resourcePtr));
}

bool RendererInterface::compSortSoftalpha(
    const SpriteDrawInfo &first, const SpriteDrawInfo &second)
{
    if (first.m_depth > second.m_depth)
    {
        return true;
    }
    else if (first.m_depth < second.m_depth)
    {
        return false;
    }

    return first.m_texture
           < second.m_texture; // Desperately try to fix performance on
                               // softalpha textures
}

}} // namespace perfectpixel::renderer
