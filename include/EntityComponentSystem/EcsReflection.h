#pragma once

#include <EntityComponentSystem/EntityManager.h>

#include <serialization/ISerializer.h>

#include <Bedrock/numbers.h>
#include <Bedrock/Singleton.h>
#include <Bedrock/Hash.h>

#include <map>

namespace perfectpixel { namespace ecs {

	class IField;

	typedef IField*(*FieldLookup)(int32_t);
	typedef bool(*HasLookup)(Entity);
	typedef void(*SerializeLookup)(serialization::ISerializer&, Entity);
	typedef void(*DeserializeLookup)(serialization::ISerializer&, Entity);
	typedef void(*RegisterLookup)(Entity);
	typedef void(*DeleteLookup)(Entity);
	typedef void(*CopyLookup)(Entity, Entity);

	struct ComponentLookup
	{
		HasLookup m_has;
		SerializeLookup m_serialize;
		DeserializeLookup m_deserialize;
		RegisterLookup m_register;
		DeleteLookup m_delete;
		FieldLookup m_fields;
		CopyLookup m_copy;
	};

	class FieldTable : public bedrock::Singleton<FieldTable>
	{
	public:
		struct ReflectionHint {};
		inline static ReflectionHint NoReflection;

		template<typename ComponentType>
		void add(
			const std::string &componentName, 
			int32_t componentId,
			const std::string &fieldName,
			int32_t fieldId,
			const std::string &typeName,
			int32_t typeId)
		{
			add<ComponentType>(componentId, fieldId, typeId);
#if PP_FULL_REFLECTION_ENABLED
			m_reverseHash[componentId] = componentName;
			m_reverseHash[fieldId] = fieldName;
			m_reverseHash[typeId] = typeName;
#endif /* PP_FULL_REFLECTION_ENABLED */
		}

		template<typename ComponentType>
		void add(
			int32_t componentId,
			int32_t fieldId,
			int32_t typeId)
		{
			ComponentLookup componentLookup;
			componentLookup.m_has = &ComponentType::Has;
			componentLookup.m_serialize = &ComponentType::Serialize;
			componentLookup.m_deserialize = &ComponentType::Deserialize;
			componentLookup.m_register = &ComponentType::VoidRegister;
			componentLookup.m_delete = &ComponentType::Delete;
			componentLookup.m_fields = &ComponentType::Lookup;
			componentLookup.m_copy = &ComponentType::Copy;
			m_componentLUT[componentId] = componentLookup;

			m_typeLUT[std::pair(componentId, fieldId)] = typeId;
		}

		int32_t componentFieldTypeID(int32_t componentId, int32_t fieldId)
		{
			return m_typeLUT[std::pair(componentId, fieldId)];
		}

		IField *getComponentFieldByID(int32_t componentId, int32_t fieldId)
		{
			return componentFieldLookupByID(componentId)(fieldId);
		}

		FieldLookup componentFieldLookupByID(int32_t componentId)
		{
			return m_componentLUT[componentId].m_fields;
		}

		HasLookup hasComponentByID(int32_t componentId)
		{
			return m_componentLUT[componentId].m_has;
		}

		std::string dequalify(const std::string &typeName)
		{
			size_t it = typeName.rfind(':');
			if (it == typeName.npos)
			{
				return typeName;
			}
			return typeName.substr(it + 1);
		}

		std::string reverse(int32_t id) const
		{
			auto it = m_reverseHash.find(id);
			if (it != m_reverseHash.end())
			{
				return it->second;
			}

			return "";
		}

		static void Touch(const std::string &str)
		{
#if PP_FULL_REFLECTION_ENABLED
			getInstance()->m_reverseHash[bedrock::crc32(str)] = str;
#endif
		}

		static std::string Reverse(int32_t id)
		{
			return getInstance()->reverse(id);
		}

		void deserialize(serialization::ISerializer &serializer, Entity entity)
		{
			serializer.readMapBegin();

			int32_t key;
			while (serializer.readMapKey(&key))
			{
				serializer.readMapBegin();

				auto &component = m_componentLUT[key];

				component.m_register(entity);
				component.m_deserialize(serializer, entity);
			}
		}

		void copy(Entity destination, Entity source)
		{
			if (EntityManager::getInstance()->isAlive(destination) &&
				EntityManager::getInstance()->isAlive(source))
			{
				for (auto it = m_componentLUT.begin(); it != m_componentLUT.end(); ++it)
				{
					if (it->second.m_has(source))
					{
						it->second.m_copy(destination, source);
					}
				}
			}
		}

		void serialize(serialization::ISerializer &serializer, Entity entity)
		{
			if (EntityManager::getInstance()->isAlive(entity))
			{
				serializer.writeMapStart();
				
				for (auto it = m_componentLUT.begin(); it != m_componentLUT.end(); ++it)
				{
					if (it->second.m_has(entity))
					{
#if PP_FULL_REFLECTION_ENABLED
						serializer.writeMapKey(m_reverseHash[it->first]);
#else
						serializer.writeMapKey(it->first);
#endif
						it->second.m_serialize(serializer, entity);
					}
				}
			}
		}

	private:
		std::map<int32_t, ComponentLookup> m_componentLUT;
		std::map<std::pair<int32_t, int32_t>, int32_t> m_typeLUT;
#if PP_FULL_REFLECTION_ENABLED
		std::map<int32_t, std::string> m_reverseHash;
#endif /* PP_FULL_REFLECTION_ENABLED */
	};

} }
