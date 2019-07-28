#pragma once

#include <Bedrock/vectors.h>
#include <Bedrock/TypeReflection.h>

namespace perfectpixel { namespace resources {

	class Image
	{
	public:
		virtual bedrock::Point2 getSize() const = 0;
		virtual uint32_t getChannels() const = 0;
		virtual unsigned char *getBuffer() const = 0;
		virtual size_t getBufferSize() const = 0;

		virtual inline ~Image() {}
	};
} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Image, Image);
