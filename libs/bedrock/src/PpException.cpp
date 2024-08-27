#include "bedrock/PpException.h"


namespace perfectpixel {
	namespace bedrock {

		PpException::PpException(const std::string &error)
			: std::runtime_error(error)
		{

		}

		PpException::~PpException()
		{
		}

	}
}
