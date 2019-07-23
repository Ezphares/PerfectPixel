#pragma once

#include <Resources/Resource.h>

#include <Resources/ResourceManager.h>

#include <EntityComponentSystem/EcsReflection.h>

#include <Bedrock/Hash.h>
#include <Bedrock/Defines.h>

namespace perfectpixel { namespace resources {

	Resource::Resource()
		: m_valid(false)
	{
	}

	Resource::Resource(const std::string &ID)
		: Resource()
	{
		set(ID);
	}

	Resource::Resource(int32_t ID)
		: Resource()
	{
		set(ID);
	}

	Resource::Resource(const Resource &toCopy)
	{
		set();
		if (toCopy.isValid())
		{
			set(toCopy.getId());
		}
	}

	Resource::Resource(Resource &&toMove)
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
		if (m_valid)
		{
			ResourceManager::Release(m_id);
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

		ResourceManager::Take(id);
		m_valid = true;
		m_id = id;
	}

} }

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &serializer, const perfectpixel::resources::Resource &resource)
{
	if (resource.isValid())
	{
		serializer.writeName(resource.getId());
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
		serializer.readName(&id);
		resource.set(id);
	}

	return serializer;
}
