#pragma once

#include <graphics/LocalGL.h>

namespace perfectpixel {
	namespace graphics {

#pragma pack (push, 1)
struct SpriteVertex {
	GLfloat x, y, z; // World coordinates
	GLfloat u, v;    // Texture coordinates
};
#pragma pack (pop)

	}
}
