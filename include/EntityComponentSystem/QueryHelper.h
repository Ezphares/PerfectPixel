#pragma once

#include <EntityComponentSystem/Query.h>
#include <EntityComponentSystem/IComponentStorage.h>

namespace perfectpixel { namespace ecs {

	class CreationDoneLifecycleComponent;
	class DestroyedLifecycleComponent;

	template<typename... Ts>
	struct ComponentTypeList;

	template<typename T, typename... Ts>
	struct ComponentTypeList<T, Ts...>
	{
		static void execute(types::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType)
		{
			T::Filter(mask, filterType);
			ComponentTypeList<Ts...>::execute(mask, filterType);
		}
	};

	template<>
	struct ComponentTypeList<>
	{
		static void execute(types::BitSet &mask, IComponentStorage::ComponentStorageFilterType filterType) {};
	};

	template<typename... Ts>
	struct With
	{
		static void execute(types::BitSet &mask)
		{
			ComponentTypeList<Ts...>::execute(mask, IComponentStorage::WITH);
		}
	};

	template<typename ...Ts>
	struct Without : public ComponentTypeList<Ts...>
	{
		static void execute(types::BitSet &mask)
		{
			ComponentTypeList<Ts...>::execute(mask, IComponentStorage::WITHOUT);
		}
	};

	template<typename WithComponents, typename WithoutComponents = typename Without<>>
	struct QueryHelper
	{
		static Query build(EntityManager *entityManager)
		{
			return Query(entityManager, [](types::BitSet &mask)
			{
				WithComponents::execute(mask);
				WithoutComponents::execute(mask);
			});
		}
	};

} }
