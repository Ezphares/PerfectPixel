#pragma once

#include "renderer/LocalGL.h"

#include <vector>

namespace perfectpixel { namespace renderer {

#pragma pack(push, 1)
struct SpriteVertex
{
    GLfloat x, y, z; // World coordinates
    GLfloat u, v;    // Texture coordinates
};

struct TextVertex
{
    GLfloat x, y, z; // World coordinates
    GLfloat u, v;    // Texture coordinates
};

struct UIVertex
{
    GLfloat x, y, z; // Screen coordinate
    GLfloat u, v;    // Texture coordinate
};
#pragma pack(pop)

typedef std::vector<SpriteVertex> SpriteBuffer;
typedef std::vector<TextVertex> TextBuffer;
typedef std::vector<UIVertex> UIBuffer;

}} // namespace perfectpixel::renderer
