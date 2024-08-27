#include "renderer/CBFGFont.h"

#include "renderer/Quad.h"

namespace perfectpixel { namespace renderer {

namespace {
const static size_t WIDTH_DATA = 20;
const static size_t IMAGE_DATA = 276;
} // namespace

CBFGFont::CBFGFont(const std::string &data)
    : m_texture(Texture::PLACEHOLDER)
{
    const char *raw = data.c_str();

    memcpy(&m_header, raw, sizeof(CBFGFontHeader));
    std::copy(raw + WIDTH_DATA, raw + IMAGE_DATA, m_width.begin());
    m_texture = Texture(m_header, &raw[IMAGE_DATA]);
    m_uvStride
        = {static_cast<float>(m_header.m_cellWidth)
               / static_cast<float>(m_header.m_imageWidth),
           static_cast<float>(m_header.m_cellHeight)
               / static_cast<float>(m_header.m_imageHeight)};
}

CBFGFont::~CBFGFont()
{}

void CBFGFont::writeBuffer(
    bedrock::Vector3 position,
    float size,
    const std::string &text,
    std::vector<TextVertex> *out_vertices)
{
    out_vertices->reserve(out_vertices->size() + text.size() * 6);

    bedrock::Vector2 cursor{position};

    for (char c : text)
    {
        int32_t row = (c - m_header.m_base)
                      / (m_header.m_imageWidth / m_header.m_cellWidth);
        int32_t col = (c - m_header.m_base)
                      % (m_header.m_imageWidth / m_header.m_cellWidth);

        float widthFactor = static_cast<float>(m_width[c])
                            / static_cast<float>(m_header.m_cellWidth);

        const Quad uvQuad{
            m_uvStride.x * col,
            m_uvStride.y * row,
            m_uvStride.x * col + (m_uvStride.x * widthFactor),
            m_uvStride.y * (row + 1)};

        const Quad xyQuad{
            cursor.x, cursor.y + size, cursor.x + widthFactor * size, cursor.y};

        TextVertex topLeft{
            xyQuad.m_left,
            xyQuad.m_top,
            position.z,
            uvQuad.m_left,
            uvQuad.m_top},
            topRight{
                xyQuad.m_right,
                xyQuad.m_top,
                position.z,
                uvQuad.m_right,
                uvQuad.m_top},
            bottomLeft{
                xyQuad.m_left,
                xyQuad.m_bottom,
                position.z,
                uvQuad.m_left,
                uvQuad.m_bottom},
            bottomRight{
                xyQuad.m_right,
                xyQuad.m_bottom,
                position.z,
                uvQuad.m_right,
                uvQuad.m_bottom};

        out_vertices->push_back(topLeft);
        out_vertices->push_back(topRight);
        out_vertices->push_back(bottomLeft);

        out_vertices->push_back(bottomLeft);
        out_vertices->push_back(topRight);
        out_vertices->push_back(bottomRight);

        cursor.x += widthFactor * size;
    }
}

perfectpixel::renderer::Texture &CBFGFont::getTexture()
{
    return m_texture;
}

float CBFGFont::getWidth(float size, const std::string &text)
{
    float width{0};
    for (auto c : text)
    {
        width += m_width[c];
    }
    return width * size / (float)m_header.m_cellWidth;
}

}} // namespace perfectpixel::renderer
