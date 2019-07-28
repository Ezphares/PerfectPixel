#pragma once

#include <Resources/Resource.h>

#include <Resources/ResourceManager.h>

#include <EntityComponentSystem/EcsReflection.h>

#include <Bedrock/Hash.h>
#include <Bedrock/Defines.h>

namespace perfectpixel { namespace resources {

	Resource::Resource(int32_t type)
		: m_type(type)
		, m_valid(false)
	{
	}

	Resource::Resource(int32_t type, const std::string &ID)
		: Resource(type)
	{
		set(ID);
	}

	Resource::Resource(int32_t type, int32_t ID)
		: Resource(type)
	{
		set(ID);
	}

	Resource::Resource(const Resource &toCopy)
		: m_type(toCopy.m_type)
	{
		set();
		if (toCopy.isValid())
		{
			set(toCopy.getId());
		}
	}

	Resource::Resource(Resource &&toMove)
		: m_type(toMove.m_type)
	{
		set();
		if (toMove.isValid())
		{
			set(toMove.getId());
		}
		toMove.set();
	}

	Resource::~Resource()
	{
		set();
	}

	Resource & Resource::operator=(const Resource &toCopy)
	{
		set();
		if (toCopy.isValid())
		{
			set(toCopy.getId());
		}
		return *this;
	}

	Resource & Resource::operator=(Resource &&toMove)
	{
		set();
		if (toMove.isValid())
		{
			set(toMove.getId());
		}
		toMove.set();
		return *this;
	}

	bool Resource::isValid() const
	{
		return m_valid;
	}

	int32_t Resource::getId() const
	{
		return m_id;
	}

	void Resource::set()
	{
		m_cache = nullptr;

		if (m_valid)
		{
			ResourceManager::Release(m_type, m_id);
		}
		m_valid = false;
	}

	void Resource::set(const std::string &id)
	{
		ecs::FieldTable::Touch(id);

		set(bedrock::crc32(id));
	}

	void Resource::set(int32_t id)
	{
		set();

		ResourceManager::Take(m_type, id);
		m_valid = true;
		m_id = id;
	}

	void *Resource::_get()
	{
		if (!m_valid)
		{
			throw bedrock::PpException("Trying to get invalid resource");
		}
		if (m_cache)
		{
			return m_cache;
		}
		bool shouldCache;
		void *result = ResourceManager::GetData(m_type, m_id, &shouldCache);

		if (result && shouldCache)
		{
			m_cache = result;
		}
		return result;
	}

} }

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &serializer, const perfectpixel::resources::Resource &resource)
{
	if (resource.isValid())
	{
		serializer.writeIdentifier(resource.getId());
	}
	else
	{
		serializer.writeNull();
	}

	return serializer;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &serializer, perfectpixel::resources::Resource &resource)
{
	resource.set();

	if (!serializer.isValueNull())
	{
		int32_t id;
		serializer.readIdentifier(&id);
		resource.set(id);
	}

	return serializer;
}
