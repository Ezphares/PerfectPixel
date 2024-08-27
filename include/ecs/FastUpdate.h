#pragma once

#include "bedrock/TypeReflection.h"

namespace perfectpixel { namespace ecs {

	struct FastUpdate
	{
        int32_t m_id;

		template <typename T>
		static FastUpdate Build()
		{
            return FastUpdate{bedrock::typeID<T>()};
		}

		static FastUpdate Null()
		{
            return FastUpdate{bedrock::typeID<std::nullptr_t>()};
		}
	};

}}
