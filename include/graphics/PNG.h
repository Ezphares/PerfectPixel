#pragma once

#include <png.h>
#include <string>

namespace perfectpixel {
namespace graphics {

	// TODO: Accessors and move stuff to private

	class PNG
	{
	public:
		struct PngReadStruct {
			png_structp m_png;
			png_infop m_pngInfo;
			png_infop m_pngEnd;
			bool m_ok;

			PngReadStruct();
			~PngReadStruct();
		};

		struct PngBuffer {
			PngBuffer();
			PngBuffer(std::size_t size);
			PngBuffer(const PngBuffer &copy);
			PngBuffer &operator=(const PngBuffer &copy);
			~PngBuffer();

			png_bytep m_idata;
			std::size_t m_size;
		};

	public:
		PNG();
		~PNG();

	public:
		static PNG fromFile(const std::string &filename, bool switchRgb = false, bool vflip = true);
		
	private:
		void closeFile();

	public:
		int m_bitDepth, m_colorType;
		png_uint_32 m_w, m_h;
		png_byte m_channels;
		PngBuffer m_buffer;

	private:
		FILE *m_fp;
	};

}
}


