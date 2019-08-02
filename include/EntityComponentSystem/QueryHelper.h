#pragma once

#include <EntityComponentSystem/Query.h>
#include <EntityComponentSystem/IComponentStorage.h>
#include <EntityComponentSystem/LifecycleComponents.h>

namespace perfectpixel { namespace ecs {

	template<typename... Ts>
	struct ComponentTypeList;

	template<typename T, typename... Ts>
	struct ComponentTypeList<T, Ts...>
	{
		static void execute(bedrock::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType)
		{
			T::Filter(mask, filterType);
			ComponentTypeList<Ts...>::execute(mask, filterType);
		}
	};

	template<>
	struct ComponentTypeList<>
	{
		static void execute(bedrock::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType) {};
	};

	template<typename... Ts>
	struct With
	{
		static void execute(bedrock::BitSet &mask)
		{
			ComponentTypeList<Ts...>::execute(mask, IComponentStorage::WITH);
		}
	};

	template<typename ...Ts>
	struct Without
	{
		static void execute(bedrock::BitSet &mask)
		{
			ComponentTypeList<Ts...>::execute(mask, IComponentStorage::WITHOUT);
		}
	};

	template<typename WithComponents, typename WithoutComponents = typename Without<>>
	struct QueryHelper
	{
		static Query build(bool onlyActive = true)
		{
			if (onlyActive) {
				return Query([](bedrock::BitSet &mask)
				{
					WithComponents::execute(mask);
					WithoutComponents::execute(mask);
				});
			}

			return Query([](bedrock::BitSet &mask)
			{
				Without<InactiveComponent>::execute(mask);
				WithComponents::execute(mask);
				WithoutComponents::execute(mask);
			});
		}
	};

} }
