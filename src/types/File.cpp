#include <types/File.h>

#include <types/PpException.h>

#include <fstream>

namespace perfectpixel {
	namespace types {

		File::File(const std::string &filename, std::ios_base::openmode mode)
			: m_buffer(NULL)
			, m_length()
		{

			if (mode & std::ios_base::in)
			{

				std::fstream stream;
				stream.open(filename.c_str(), mode);
				stream.seekg(0, stream.end);
				std::streamoff length = stream.tellg();
				stream.seekg(0, stream.beg);

				m_buffer = (char*)malloc(static_cast<size_t>(length + 1));
				if (m_buffer == NULL)
				{
					throw types::PpException("Could not allocate file buffer");
				}

				stream.read(m_buffer, length);
				m_buffer[length] = '\x00';

				m_length = static_cast<size_t>(length);
			}
		}

		File::~File()
		{
			delete m_buffer;
		}

		std::string File::str()
		{
			if (m_buffer == NULL)
			{
				return std::string("");
			}
			return std::string(m_buffer, m_length);
		}

	}
}
