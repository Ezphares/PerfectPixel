#include <graphics/PNG.h>

#include <types/PpException.h>

namespace perfectpixel {
namespace graphics {

	PNG::PNG()
		: m_bitDepth(0)
		, m_colorType(0)
		, m_h(0)
		, m_w(0)
		, m_channels(0)
		, m_buffer()
	{
	}

	PNG::~PNG()
	{
		closeFile();
	}

	PNG PNG::fromFile(const std::string &filename, bool switchRgb, bool vflip)
	{
		PNG result;

		fopen_s(&result.m_fp, filename.c_str(), "rb");
		if (result.m_fp == NULL)
		{
			throw types::PpException("Could not open file [" + filename + "]");
		}

		// PNG header check
		png_byte header[8];
		fread(header, 1, 8, result.m_fp);
		if (png_sig_cmp(header, 0, 8))
		{
			throw types::PpException("Could not parse PNG header");
		}

		PngReadStruct readStruct = PngReadStruct();

		if (!readStruct.m_ok)
		{
			throw types::PpException("Could not create PNG metadata structures");
		}

		if (setjmp(png_jmpbuf(readStruct.m_png)))
		{
			throw types::PpException("Could not set jump");// FIXME exception
		}

		png_init_io(readStruct.m_png, result.m_fp);
		png_set_sig_bytes(readStruct.m_png, 8);
		png_read_info(readStruct.m_png, readStruct.m_pngInfo);

		png_get_IHDR(
			readStruct.m_png,
			readStruct.m_pngInfo,
			&result.m_w, 
			&result.m_h, 
			&result.m_bitDepth, 
			&result.m_colorType, 
			NULL, NULL, NULL);

		result.m_channels = png_get_channels(readStruct.m_png, readStruct.m_pngInfo);

		if (switchRgb)
		{
			png_set_bgr(readStruct.m_png);
		}
		png_read_update_info(readStruct.m_png, readStruct.m_pngInfo);

		size_t rowbytes = png_get_rowbytes(readStruct.m_png, readStruct.m_pngInfo);

		result.m_buffer = PngBuffer(rowbytes * result.m_h);
		png_bytepp rowpointers = new png_bytep[result.m_h];
		for (unsigned int i = 0; i < result.m_h; ++i)
		{
			unsigned int index = vflip ? result.m_h - 1 - i : i;
			rowpointers[index] = result.m_buffer.m_idata + i * rowbytes;
		}
		png_read_image(readStruct.m_png, rowpointers);

		delete[] rowpointers;
		result.closeFile();

		return result;
	}

	void PNG::closeFile()
	{
		if (m_fp)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
	}

	PNG::PngReadStruct::PngReadStruct()
	{
		m_ok = false;

		m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (m_png == NULL) return;

		m_pngInfo = png_create_info_struct(m_png);
		if (m_pngInfo == NULL) return;

		m_pngEnd = png_create_info_struct(m_png);
		if (m_pngEnd == NULL) return;

		m_ok = true;
	}

	PNG::PngReadStruct::~PngReadStruct()
	{
		png_destroy_read_struct(&m_png, &m_pngInfo, &m_pngEnd);
	}

	PNG::PngBuffer::PngBuffer(std::size_t size)
		: m_idata (static_cast<png_bytep>(malloc(sizeof(png_bytep) * size)))
		, m_size(size)
	{
	}

	PNG::PngBuffer::PngBuffer(const PngBuffer &copy)
		: m_idata(NULL)
		, m_size(copy.m_size)
	{
		if (copy.m_idata != NULL)
		{
			m_idata = static_cast<png_bytep>(malloc(sizeof(png_bytep) * copy.m_size));
			if (m_idata)
			{
				memcpy(m_idata, copy.m_idata, sizeof(png_bytep) * copy.m_size);
			}
		}
	}

	PNG::PngBuffer::PngBuffer()
		: m_idata(NULL)
		, m_size(0)
	{

	}

	PNG::PngBuffer::~PngBuffer()
	{
		if (m_idata)
		{
			free(m_idata);
		}
	}

	PNG::PngBuffer &PNG::PngBuffer::operator=(const PngBuffer &copy)
	{
		if (this != &copy)
		{
			delete m_idata;
			m_size = copy.m_size;
			m_idata = NULL;

			if (copy.m_idata != NULL)
			{
				m_idata = static_cast<png_bytep>(malloc(sizeof(png_bytep) * copy.m_size));
				if (m_idata)
				{
					memcpy(m_idata, copy.m_idata, sizeof(png_bytep) * copy.m_size);
				}
			}
		}

		return *this;
	}

}
}
