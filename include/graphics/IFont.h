#pragma once

#include <graphics/BufferLayouts.h>

#include <types/vectors.h>

#include <string>
#include <vector>

namespace perfectpixel {
	namespace graphics {
		
		class Texture;

		class IFont {
		public:
			virtual ~IFont() {};

			virtual void writeBuffer(types::Vector3 position, types::PpFloat size, const std::string &text, std::vector<TextVertex> *out_vertices) = 0;
			virtual Texture &getTexture() = 0;
			virtual types::PpFloat getWidth(types::PpFloat size, const std::string &text) = 0;
		};
	}
}



