#pragma once

#include <managerinterface/Query.h>

namespace perfectpixel { namespace api {

	template <typename... Ts>
	struct QueryResult;

	template <typename T, typename... Ts>
	struct QueryResult<T, Ts...> : public QueryResult<Ts...>
	{
		QueryResult(world::Entity entity)
			: QueryResult<Ts...>(entity)
		{}

		template<typename S = typename std::enable_if<T == S, T>::type>
		S *get() { return world::ComponentRegistry::Instance()->getComponent<S>(m_entity); }
	};

	template <>
	struct QueryResult<>
	{
		QueryResult(world::Entity entity)
			: m_entity(entity)
		{}

		world::Entity m_entity;
	};

	template<typename... Ts>
	struct ComponentTypeList;

	template<typename T, typename... Ts>
	struct ComponentTypeList<T, Ts...>
	{
		static void getTypes(std::vector<world::ComponentTypeId> &out_types) 
		{
			if (out_types.capacity() < 1 + sizeof(Ts))
			{
				out_types.reserve(1 + sizeof(Ts));
			}
			
			out_types.push_back(T::getTypeId());
			ComponentTypeList<Ts...>::getTypes(out_types);
		}
	};

	template<>
	struct ComponentTypeList<>
	{
		static void getTypes(std::vector<world::ComponentTypeId> &)
		{
			// Recursion base
		}
	};

	template<typename... Ts>
	struct With : public ComponentTypeList<Ts...>
	{
		typedef QueryResult<Ts...> QueryResultType;
	};

	template<typename ...Ts>
	struct Without : public ComponentTypeList<Ts...>
	{
	};

	template<typename WithComponents, typename WithoutComponents>
	struct QueryHelper
	{
		typedef typename WithComponents::QueryResultType ResultType;
		typedef std::vector<ResultType> ResultSet;

		static Query build()
		{
			std::vector<world::ComponentTypeId> with, without;
			WithComponents::getTypes(with);
			WithoutComponents::getTypes(without);

			Query query;

			for (auto id : with)
			{
				query.with(id);
			}
			for (auto id : without)
			{
				query.without(id);
			}

			return query;
		}

		static ResultType pack(world::Entity entity)
		{
			return ResultType(entity);
		}

		static void packAll(const std::vector<world::Entity> entities, ResultSet &out_results)
		{
			out_results.reserve(entities.size());
			out_results.clear();
			for (auto entity : entities)
			{
				out_results.emplace_back(pack(entity));
			}
		}
	};

} }
