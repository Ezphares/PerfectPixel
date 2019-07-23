#pragma once

#include <serialization/ISerializer.h>

#include <Bedrock/TypeReflection.h>

#include <string>

namespace perfectpixel { namespace resources {

	class Resource
	{
	public:
		Resource();
		Resource(const std::string &id);
		Resource(int32_t id);

		~Resource();
		Resource(const Resource &toCopy);
		Resource(Resource &&toMove);
		Resource &operator=(const Resource &toCopy);
		Resource &operator=(Resource &&toMove);

		bool isValid() const;
		int32_t getId() const;

		void set(const std::string &id);
		void set(int32_t id);
		void set();

	private:
		int32_t m_id;
		bool m_valid;
	};
} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Resource, Resource);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &serializer, const perfectpixel::resources::Resource &resource);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &serializer, perfectpixel::resources::Resource &resource);
