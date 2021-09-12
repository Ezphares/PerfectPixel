#pragma once

#include <EntityComponentSystem/IComponentStorage.h>
#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/Query.h>

namespace perfectpixel { namespace ecs {

template <typename... Ts>
struct ComponentTypeList;

template <typename T, typename... Ts>
struct ComponentTypeList<T, Ts...>
{
    static void
    execute(bedrock::BitSet &mask, ComponentStorageFilterType filterType)
    {
        T::Filter(mask, filterType);
        ComponentTypeList<Ts...>::execute(mask, filterType);
    }

    static bool dirty(uint32_t referenceFrame)
    {
        if (T::DirtyFrame() > referenceFrame)
        {
            return true;
        }

        return ComponentTypeList<Ts...>::dirty(referenceFrame);
    }
};

template <>
struct ComponentTypeList<>
{
    static void
    execute(bedrock::BitSet &mask, ComponentStorageFilterType filterType)
    {
        (void)mask;
        (void)filterType;
    };

    static bool dirty(uint32_t referenceFrame)
    {
        (void)referenceFrame;
        return false;
    }
};

template <typename... Ts>
struct With
{
    static bool dirty(uint32_t referenceFrame)
    {
        return ComponentTypeList<Ts...>::dirty(referenceFrame);
    }

    static void execute(bedrock::BitSet &mask)
    {
        ComponentTypeList<Ts...>::execute(
            mask, ComponentStorageFilterType::WITH);
    }
};

template <typename... Ts>
struct Without
{
    static bool dirty(uint32_t referenceFrame)
    {
        return ComponentTypeList<Ts...>::dirty(referenceFrame);
    }

    static void execute(bedrock::BitSet &mask)
    {
        ComponentTypeList<Ts...>::execute(
            mask, ComponentStorageFilterType::WITHOUT);
    }
};

template <typename WithComponents, typename WithoutComponents = Without<>>
struct QueryHelper
{
    static Query build(bool onlyActive = true)
    {
        if (onlyActive)
        {
            return Query(
                [](bedrock::BitSet &mask) {
                    Without<InactiveComponent>::execute(mask);
                    WithComponents::execute(mask);
                    WithoutComponents::execute(mask);
                },
                [](uint32_t referenceFrame) {
                    return Without<InactiveComponent>::dirty(referenceFrame)
                           || WithComponents::dirty(referenceFrame)
                           || WithoutComponents::dirty(referenceFrame);
                });
        }

        return Query(
            [](bedrock::BitSet &mask) {
                WithComponents::execute(mask);
                WithoutComponents::execute(mask);
            },
            [](uint32_t referenceFrame) {
                return WithComponents::dirty(referenceFrame)
                       || WithoutComponents::dirty(referenceFrame);
            });
    }
};

}} // namespace perfectpixel::ecs
