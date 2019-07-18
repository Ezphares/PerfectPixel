#pragma once

#include <string>
#include <iostream>

namespace perfectpixel { namespace bedrock {

class File
{
public:

	File(const std::string &filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary);
	~File();

public:
	std::string str();

private:
	char *m_buffer;
	size_t m_length;
};

}
}
