#pragma once

#include <Bedrock/numbers.h>
#include <Bedrock/Singleton.h>
#include <Bedrock/Hash.h>

#include <map>

namespace perfectpixel { namespace ecs {

	class IField;

	typedef IField*(*FieldLookup)(int32_t);
	typedef bool(*HasLookup)(int32_t);

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
			m_componentFieldLUT[componentId] = &ComponentType::Lookup;
			m_typeLUT[std::pair(PP_ID(componentName), PP_ID(fieldName))] = typeId;
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
			return m_componentFieldLUT[componentId];
		}

		HasLookup hasComponentByID(int32_t componentId)
		{
			return m_hasComponentLUT[componentId];
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

	private:
		std::map<int32_t, FieldLookup> m_componentFieldLUT;
		std::map<int32_t, HasLookup> m_hasComponentLUT;
		std::map<std::pair<int32_t, int32_t>, int32_t> m_typeLUT;
#if PP_FULL_REFLECTION_ENABLED
		std::map<int32_t, std::string> m_reverseHash;
#endif /* PP_FULL_REFLECTION_ENABLED */
	};

} }
