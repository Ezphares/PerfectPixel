#pragma once

#include <graphics/LocalGL.h>

namespace perfectpixel {
	namespace graphics {

#pragma pack (push, 1)
struct SpriteVertex {
	GLfloat x, y, z; // World coordinates
	GLfloat u, v;    // Texture coordinates
};

struct TextVertex {
	GLfloat x, y, z; // World coordinates
	GLfloat u, v;    // Texture coordinates
};
#pragma pack (pop)

typedef std::vector<SpriteVertex> SpriteBuffer;
typedef std::vector<TextVertex> TextBuffer;


	}
}
