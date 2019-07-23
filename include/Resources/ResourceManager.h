#pragma once

#include <Bedrock/Singleton.h>

#include <string>

namespace perfectpixel { namespace resources {

	class ResourceManager : public bedrock::Singleton<ResourceManager>
	{
	public:
		static void Take(int32_t id);
		static void Release(int32_t id);
	};

} }

