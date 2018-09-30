#pragma once

#include <graphics/LocalGL.h>
#include <graphics/PNG.h>

#include <types/vectors.h>

#include <string>

namespace perfectpixel {
namespace graphics {

class Texture
{
public:
	struct PlaceHolder {};
	static const PlaceHolder PLACEHOLDER;
	
public:
	Texture(const PlaceHolder &);
	Texture(const PNG &png);
	Texture(const types::Point2 size);
	~Texture();

public:
	void bind();
	types::Vector2 pixelToTexture(types::Point2 pixel) const;
	types::Point2 textureToPixel(types::Vector2 pixel) const;

	GLuint getId() const;

	void destroy();

private:
	GLuint m_textureId;
	types::Point2 m_size;

};

}
}

