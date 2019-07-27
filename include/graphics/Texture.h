#pragma once

#include <graphics/LocalGL.h>

#include <Resources/Image.h>

#include <Bedrock/vectors.h>

#include <string>

namespace perfectpixel {
namespace graphics {

	// Forward declared
	struct CBFGFontHeader;


class Texture
{
public:
	struct PlaceHolder {};
	static const PlaceHolder PLACEHOLDER;
	
public:
	Texture(const PlaceHolder &);
	Texture(const resources::Image &image);
	Texture(const bedrock::Point2 size);
	Texture(const CBFGFontHeader& header, const char *raw);

	~Texture();

public:
	void bind(GLuint unit = 1);
	static void unbind(GLuint unit = 1);
	bedrock::Vector2 pixelToTexture(bedrock::Point2 pixel) const;
	bedrock::Point2 textureToPixel(bedrock::Vector2 pixel) const;

	GLuint getId() const;

	void destroy();

private:
	GLuint m_textureId;
	bedrock::Point2 m_size;

};

}
}

