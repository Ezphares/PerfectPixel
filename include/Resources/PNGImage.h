#pragma once

#include <Resources/Image.h>

#include <Bedrock/Opaque.h>

#include <png.h>

namespace perfectpixel {
	namespace resources {

		class PNGImage : public Image
		{
		public:
			struct PNGImageUserData
			{
				bool m_bgr, m_vflip;
			};

			static void PNGImageLoaderFunction(char *data, size_t dataSize, void **target, const bedrock::Opaque &userData)
			{
				const PNGImageUserData *udata = userData.get<PNGImageUserData>();
				*target = new PNGImage();
				PNGImage::FromSimpleBuffer(
					reinterpret_cast<PNGImage *>(*target),
					data, 
					dataSize,
					udata ? udata->m_bgr : false,
					udata ? udata->m_vflip : true
				);
			}


			virtual bedrock::Point2 getSize() const override;
			virtual uint32_t getChannels() const override;
			virtual unsigned char * getBuffer() const override;
			virtual size_t getBufferSize() const override;

		private:
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

			struct SimpleBuffer {
				char *m_buffer;
				size_t m_size;
				size_t m_offset;
			};

		public:
			PNGImage();

		public:
			static PNGImage fromFile(const std::string &filename, bool switchRgb = false, bool vflip = true);

			static void FromSimpleBuffer(PNGImage *out_image, char *buffer, size_t bufferSize, bool bgr, bool vflip);
			static void ReadFromSimpleBuffer(png_structp png_ptr, png_bytep out_bytes, png_size_t count);

		public:
			int m_bitDepth, m_colorType;
			png_uint_32 m_w, m_h;
			png_byte m_channels;
			PngBuffer m_buffer;
		};

	}
}
