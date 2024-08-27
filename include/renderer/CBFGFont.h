#pragma once

/*******************************************************************************
This loads fonts generated with the tool Codehead's Bitmap Font Generator (CBFG)
 a cool piece of software created by Karl Walsh
The tool is available at https://www.codehead.co.uk/cbfg/
 and https://github.com/CodeheadUK/CBFG
****************************************************/

#include "renderer/IFont.h"
#include "renderer/Texture.h"

#include <array>

namespace perfectpixel { namespace renderer {

#pragma pack(push, 1)
struct CBFGFontHeader
{
    unsigned char m_id[2];
    uint32_t m_imageWidth;
    uint32_t m_imageHeight;
    uint32_t m_cellWidth;
    uint32_t m_cellHeight;
    unsigned char m_bpp;
    char m_base;
};
#pragma pack(pop)

class CBFGFont : public IFont
{
public:
    CBFGFont(const std::string &data);
    ~CBFGFont();

public:
    virtual void writeBuffer(
        bedrock::Vector3 position,
        float size,
        const std::string &text,
        std::vector<TextVertex> *out_vertices);
    virtual Texture &getTexture();
    virtual float getWidth(float size, const std::string &text);

private:
    Texture m_texture;
    CBFGFontHeader m_header;
    std::array<unsigned char, 256> m_width;
    bedrock::Vector2 m_uvStride;
};

}} // namespace perfectpixel::renderer
