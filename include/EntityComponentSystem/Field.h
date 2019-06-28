#pragma once

#include <EntityComponentSystem/Entity.h>

#include <vector>

namespace perfectpixel { namespace ecs {

	class IField
	{
	public:
		virtual void reset(uint32_t index) = 0;
	};

	template <typename Owner, typename T>
	class Field : public IField
	{
	public:
		Field()
			: m_data()
		{
			Owner::AddField(this);
		}

		T at(uint32_t idx)
		{
			return m_data[idx];
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

		virtual void reset(uint32_t idx)
		{
			if (m_data.size() <= idx)
			{
				m_data.resize(idx + 1);
			}
			m_data[idx] = T();
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

		ArrayField()
		{
			Owner::AddField(this);
		}

		T at(uint32_t idx)
		{
			return m_data[idx];
		}

		const T ^get(Entity entity) const
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

	private:
		std::vector<std::vector<T>> m_data;
	};

} }
