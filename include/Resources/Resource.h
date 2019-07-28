#pragma once

#include <serialization/ISerializer.h>

#include <Bedrock/TypeReflection.h>

#include <string>

namespace perfectpixel { namespace resources {

	class Resource
	{
	public:
		Resource(int32_t type);
		Resource(int32_t type, const std::string &id);
		Resource(int32_t type, int32_t id);

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

		void *_get();
		template<typename T>
		T *get()
		{
			return reinterpret_cast<T *>(_get());
		}

	private:
		const int32_t m_type;
		int32_t m_id;
		uint32_t m_cacheHint;
		void *m_cache;
		bool m_valid;
	};
} }

PP_TYPE_REFLECTION(::perfectpixel::resources::Resource, Resource);

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &serializer, const perfectpixel::resources::Resource &resource);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &serializer, perfectpixel::resources::Resource &resource);
