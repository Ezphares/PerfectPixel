#pragma once

#include <string>
#include <iostream>

namespace perfectpixel { namespace types {

class File
{
public:

	File(const std::string &filename, std::ios_base::open_mode mode = std::ios_base::in | std::ios_base::binary);
	~File();

public:
	std::string str();

private:
	char *m_buffer;
};

}
}
