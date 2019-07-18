#pragma once

#include <Bedrock/numbers.h>
#include <Bedrock/Singleton.h>
#include <Bedrock/Hash.h>

#include <map>

namespace perfectpixel { namespace ecs {

	class IField;

	typedef IField*(*FieldLookup)(bedrock::PpInt);
	typedef bool(*HasLookup)(bedrock::PpInt);

	class FieldTable : public bedrock::Singleton<FieldTable>
	{
	public:
		struct ReflectionHint {};
		inline static ReflectionHint NoReflection;

		template<typename ComponentType>
		void add(
			const std::string &componentName, 
			bedrock::PpInt componentId,
			const std::string &fieldName,
			bedrock::PpInt fieldId,
			const std::string &typeName,
			bedrock::PpInt typeId)
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
			bedrock::PpInt componentId,
			bedrock::PpInt fieldId,
			bedrock::PpInt typeId)
		{
			m_componentFieldLUT[componentId] = &ComponentType::Lookup;
			m_typeLUT[std::pair(PP_ID(componentName), PP_ID(fieldName))] = typeId;
		}

		bedrock::PpInt componentFieldTypeID(bedrock::PpInt componentId, bedrock::PpInt fieldId)
		{
			return m_typeLUT[std::pair(componentId, fieldId)];
		}

		IField *getComponentFieldByID(bedrock::PpInt componentId, bedrock::PpInt fieldId)
		{
			return componentFieldLookupByID(componentId)(fieldId);
		}

		FieldLookup componentFieldLookupByID(bedrock::PpInt componentId)
		{
			return m_componentFieldLUT[componentId];
		}

		HasLookup hasComponentByID(bedrock::PpInt componentId)
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
		std::map<bedrock::PpInt, FieldLookup> m_componentFieldLUT;
		std::map<bedrock::PpInt, HasLookup> m_hasComponentLUT;
		std::map<std::pair<bedrock::PpInt, bedrock::PpInt>, bedrock::PpInt> m_typeLUT;
#if PP_FULL_REFLECTION_ENABLED
		std::map<bedrock::PpInt, std::string> m_reverseHash;
#endif /* PP_FULL_REFLECTION_ENABLED */
	};

} }
