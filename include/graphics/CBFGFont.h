#pragma once

/*******************************************************************************
This loads fonts generated with the tool Codehead's Bitmap Font Generator (CBFG)
 a cool piece of software created by Karl Walsh
The tool is available at https://www.codehead.co.uk/cbfg/ 
 and https://github.com/CodeheadUK/CBFG 
****************************************************/

#include <graphics/Texture.h>
#include <graphics/IFont.h>

#include <array>

namespace perfectpixel {
	namespace graphics {

#pragma pack(push, 1)
		struct CBFGFontHeader {
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
			virtual void writeBuffer(types::Vector3 position, types::PpFloat size, const std::string &text, std::vector<TextVertex> *out_vertices);
			virtual Texture &getTexture();
			virtual types::PpFloat getWidth(types::PpFloat size, const std::string &text);

		private:
			Texture m_texture;
			CBFGFontHeader m_header;
			std::array<unsigned char, 256> m_width;
			types::Vector2 m_uvStride;
		};

	}
}
