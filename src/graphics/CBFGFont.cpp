#include <graphics/CBFGFont.h>

#include <graphics/Quad.h>

namespace perfectpixel {
	namespace graphics {

		namespace
		{
			const static size_t WIDTH_DATA = 20;
			const static size_t IMAGE_DATA = 276;
		}


		CBFGFont::CBFGFont(const std::string &data)
			: m_texture(Texture::PLACEHOLDER)
		{
			const char *raw = data.c_str();

			memcpy(&m_header, raw, sizeof(CBFGFontHeader));
			std::copy(raw + WIDTH_DATA, raw + IMAGE_DATA, m_width.begin());
			m_texture = Texture(m_header, &raw[IMAGE_DATA]);
			m_uvStride = {
				static_cast<types::PpFloat>(m_header.m_cellWidth) / static_cast<types::PpFloat>(m_header.m_imageWidth),
				static_cast<types::PpFloat>(m_header.m_cellHeight) / static_cast<types::PpFloat>(m_header.m_imageHeight)
			};
		}

		CBFGFont::~CBFGFont()
		{
		}

		void CBFGFont::writeBuffer(types::Vector3 position, types::PpFloat size, const std::string &text, std::vector<TextVertex> *out_vertices)
		{
			out_vertices->reserve(out_vertices->size() + text.size() * 6);

			types::Vector2 cursor{ position };

			for (char c : text)
			{
				types::PpInt row = (c - m_header.m_base) / (m_header.m_imageWidth / m_header.m_cellWidth);
				types::PpInt col = (c - m_header.m_base) % (m_header.m_imageWidth / m_header.m_cellWidth);

				types::PpFloat widthFactor = static_cast<types::PpFloat>(m_width[c]) / static_cast<types::PpFloat>(m_header.m_cellWidth);

				const Quad uvQuad{
					m_uvStride.x() * col,
					m_uvStride.y() * row,
					m_uvStride.x() * col + (m_uvStride.x() * widthFactor),
					m_uvStride.y() * (row + 1)
				};

				const Quad xyQuad{
					cursor.x(),
					cursor.y() + size,
					cursor.x() + widthFactor * size,
					cursor.y()
				};

				TextVertex
					topLeft{ xyQuad.m_left, xyQuad.m_top, position.z(), uvQuad.m_left, uvQuad.m_top },
					topRight{ xyQuad.m_right, xyQuad.m_top, position.z(), uvQuad.m_right, uvQuad.m_top },
					bottomLeft{ xyQuad.m_left, xyQuad.m_bottom, position.z(), uvQuad.m_left, uvQuad.m_bottom },
					bottomRight{ xyQuad.m_right, xyQuad.m_bottom, position.z(), uvQuad.m_right, uvQuad.m_bottom };

				out_vertices->push_back(topLeft);
				out_vertices->push_back(topRight);
				out_vertices->push_back(bottomLeft);

				out_vertices->push_back(bottomLeft);
				out_vertices->push_back(topRight);
				out_vertices->push_back(bottomRight);

				cursor.x() += widthFactor * size;
			}
		}

		perfectpixel::graphics::Texture & CBFGFont::getTexture()
		{
			return m_texture;
		}

		perfectpixel::types::PpFloat CBFGFont::getWidth(types::PpFloat size, const std::string &text)
		{
			types::PpFloat width{ 0 };
			for (auto c : text)
			{
				width += m_width[c];
			}
			return width * size;
		}

	}
}
