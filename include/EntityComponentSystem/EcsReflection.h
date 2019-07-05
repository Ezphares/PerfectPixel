#pragma once

#include <types/numbers.h>
#include <types/Singleton.h>
#include <types/Hash.h>

#include <map>

namespace perfectpixel { namespace ecs {

	class IField;

	typedef IField*(*FieldLookup)(types::PpInt);

	class FieldTable : public types::Singleton<FieldTable>
	{
	public:
		struct ReflectionHint {};
		inline static ReflectionHint NoReflection;

		template<typename ComponentType>
		void add(
			const std::string &componentName, 
			types::PpInt componentId,
			const std::string &fieldName,
			types::PpInt fieldId,
			const std::string &typeName,
			types::PpInt typeId)
		{
			std::string rawType = dequalify(typeName);
			add<ComponentType>(componentId, fieldId, types::PpId(dequalify(rawType)));
		}

		template<typename ComponentType>
		void add(
			types::PpInt componentId,
			types::PpInt fieldId,
			types::PpInt typeId)
		{
			m_componentLUT[componentId] = &ComponentType::Lookup;
			m_typeLUT[std::pair(PPID(componentName), PPID(fieldName))] = typeId;
		}

		IField *find(const std::string &componentName, const std::string &fieldName)
		{
			return m_componentLUT[types::PpId(componentName)](types::PpId(fieldName));
		}

		types::PpInt getType(const std::string &componentName, const std::string &fieldName)
		{
			return m_typeLUT[std::pair(types::PpId(componentName), types::PpId(fieldName))];
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
		std::map<types::PpInt, FieldLookup> m_componentLUT;
		std::map<std::pair<types::PpInt, types::PpInt>, types::PpInt> m_typeLUT;
	};

} }
