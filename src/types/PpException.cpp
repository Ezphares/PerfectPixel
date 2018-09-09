#include <types/PpException.h>


namespace perfectpixel {
	namespace types {

		PpException::PpException(const std::string &error)
			: std::runtime_error(error)
		{

		}

		PpException::~PpException()
		{
		}

	}
}
