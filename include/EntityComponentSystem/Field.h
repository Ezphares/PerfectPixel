#pragma once

#include <EntityComponentSystem/EcsReflection.h>

#include <serialization/BinarySerializer.h>

#include <EntityComponentSystem/Entity.h>
#include <Bedrock/defines.h>
#include <Bedrock/numbers.h>
#include <Bedrock/Hash.h>

#include <vector>
#include <type_traits>

#define PPFIELDTYPE(Owner, T) ::perfectpixel::ecs::Field<Owner, T>
#define PPARRAYFIELDTYPE(Owner, T, C) ::perfectpixel::ecs::ArrayField<Owner, T, C>

#if PP_FULL_REFLECTION_ENABLED

#define _Field(Owner, T, Name) inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T) \
(PP_DEQUALIFY(Owner), PP_DQID(Owner), #Name, PP_ID(Name), ""/*bedrock::typeName<T>()*/, 0 /*bedrock::typeID<T>()*/);

#define _ArrayField(Owner, T, Capacity, Name) inline static PPARRAYFIELDTYPE(Owner, T, Capacity) Name = PPARRAYFIELDTYPE(Owner, T, Capacity) \
(PP_DEQUALIFY(Owner), PP_DQID(Owner), #Name, PP_ID(Name), PP_DEQUALIFY(T), PP_DQID(T));

#else

#define _Field(Owner, T, Name) inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(PP_DQID(Owner), PP_ID(Name), PP_DQID(T));

#define _ArrayField(Owner, T, Capacity, Name) inline static PPFIELDTYPE(Owner, T, Capacity) Name = PPFIELDTYPE(Owner, T, Capacity)(PP_DQID(Owner), PP_ID(Name), PP_DQID(T));

#endif

#define _InternalField(Owner, T, Name) inline static PPFIELDTYPE(Owner, T) Name = PPFIELDTYPE(Owner, T)(::perfectpixel::ecs::FieldTable::NoReflection);

namespace perfectpixel { namespace ecs {

	class IField
	{
	public:
		virtual void reset(uint32_t index = 0) = 0;
		virtual void serialize(serialization::ISerializer &serializer, uint32_t index) = 0;
		virtual void deserialize(serialization::ISerializer &serializer, uint32_t index) = 0;
	};

	template <typename Owner, typename T>
	class Field : public IField
	{

	public:
		Field(FieldTable::ReflectionHint)
			: m_data()
		{
		}

		Field(
			int32_t ownerId,
			int32_t selfId,
			int32_t typeId)
			: m_data()
		{
			if (Owner::AddField(selfId, this))
			{
				FieldTable::getInstance()->add<Owner>(ownerId, selfId, typeId);
			}
		}

#if PP_FULL_REFLECTION_ENABLED
		Field(
			const std::string &ownerName,
			int32_t ownerId,
			const std::string &selfName,
			int32_t selfId,
			const std::string &typeName, 
			int32_t typeId)
			: m_data()
		{
			if (Owner::AddField(selfId, this))
			{
				FieldTable::getInstance()->add<Owner>(ownerName, ownerId, selfName, selfId, typeName, typeId);
			}
		}
#endif

		T at(uint32_t idx)
		{
			return m_data[idx];
		}

		void _set(uint32_t idx, const T &val)
		{
			m_data[idx] = val;
		}

		T Get(Entity entity) const
		{
			return m_data[Owner::Index(entity)];
		}

		void Set(Entity entity, const T& value)
		{
			m_data[Owner::Index(entity)] = false;
		}

		// Raw access operator
		T &operator()(Entity entity)
		{
			return m_data[Owner::Index(entity)];
		}

		const T &operator()(Entity entity) const
		{
			return m_data.at(Owner::Index(entity));
		}

		virtual void reset(uint32_t index)
		{
			if (m_data.size() <= index)
			{
				m_data.resize(index + 1);
			}
			m_data[index] = T();
		}

		virtual void serialize(serialization::ISerializer &serializer, uint32_t index)
		{
			serializer << m_data[index];
		}

		virtual void deserialize(serialization::ISerializer &serializer, uint32_t index)
		{
			_deserialize<T>(serializer, index);
		}

		// For overloading reasons we have to split the deserialization into enums and everything else
		template <typename X,
			typename std::enable_if_t<!std::is_enum<X>::value>* = nullptr>
		void _deserialize(serialization::ISerializer &serializer, uint32_t index)
		{
			serializer >> m_data[index];
		}

		template <typename X,
			typename std::enable_if_t<std::is_enum<X>::value>* = nullptr>
			void _deserialize(serialization::ISerializer &serializer, uint32_t index)
		{
			std::underlying_type<T>::type temp;
			serializer >> temp;
			m_data[index] = static_cast<T>(temp);
		}

	private:
		std::vector<T> m_data;
	};

	template <typename Owner, typename T, std::uint32_t Capacity>
	class ArrayField : public IField
	{
	public:
		// FIXME Use a better container type
		typedef std::vector<T> Container;

		ArrayField(FieldTable::ReflectionHint)
			: m_data
		{
		}

		ArrayField(
			int32_t ownerId,
			int32_t selfId,
			int32_t typeId)
			: m_data()
		{
			if (Owner::AddField(selfId, this))
			{
				FieldTable::getInstance()->add<Owner>(ownerId, selfId, typeId);
			}
		}

#if PP_FULL_REFLECTION_ENABLED
		ArrayField(
			const std::string &ownerName,
			int32_t ownerId,
			const std::string &selfName,
			int32_t selfId,
			const std::string &typeName,
			int32_t typeId)
			: m_data()
		{
			if (Owner::AddField(selfId, this))
			{
				FieldTable::getInstance()->add<Owner>(ownerName, ownerId, selfName, selfId, typeName, typeId);
			}
		}
#endif

		T at(uint32_t idx)
		{
			return m_data[idx];
		}

		const T &get(Entity entity) const
		{
			return m_data[Owner::Index(entity)];
		}

		void set(Entity entity, const T& value)
		{
			m_data[Owner::Index(entity)] = value;
		}

		uint32_t capacity(Entity entity)
		{
			return m_data[Owner::Index(entity)].size();
		}

		// Raw access operator
		Container &operator()(Entity entity)
		{
			return m_data[Owner::Index(entity)];
		}

		T &operator()(Entity entity, uint32_t index)
		{
			return m_data[Owner::Index(entity)][index];
		}

		const Container &operator()(Entity entity) const
		{
			return m_data.at(Owner::Index(entity));
		}

		const T &operator()(Entity entity, uint32_t index) const
		{
			return m_data.at(Owner::Index(entity)).at(index);
		}

		virtual void reset(uint32_t idx)
		{
			if (m_data.size() <= idx)
			{
				m_data.resize(idx + 1);
			}
			m_data[idx] = std::vector<T>();
		}

		virtual void serialize(serialization::ISerializer &serializer, uint32_t index)
		{
			serializer.writeArrayStart();
			//TODO
			serializer.writeArrayEnd();
		}

		virtual void deserialize(serialization::ISerializer &serializer, uint32_t index)
		{
			uint32_t arraySize = serializer.readArrayStart();
			// TODO
			serializer.readArrayEnd();
		}

	private:
		std::vector<std::vector<T>> m_data;
	};

} }
