#pragma once

#include <stdexcept>
#include <string>

namespace perfectpixel {
	namespace bedrock{

class PpException : public std::runtime_error
{
public:
	PpException(const std::string &error);
	~PpException();


};

	}
}
