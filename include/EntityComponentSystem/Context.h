#pragma once

#include "EntityComponentSystem/Manager.h"

#include "Bedrock/TypeReflection.h"

#include <memory>
#include <unordered_map>

namespace pp::ecs {
class Context
{
    using ManagerPtr = std::unique_ptr<void, void(*)(void*)>;
    using ManagerContainer
        = std::unordered_map<perfectpixel::bedrock::TypeID, ManagerPtr>;

public:
    template <typename Component>
    Manager<Component> &getManager()
    {
        ManagerContainer::iterator itr
            = managers.find(perfectpixel::bedrock::typeID<Component>());

        if (itr == managers.end())
        {
            itr = createManager<Component>();
        }

        return *static_cast<Manager<Component> *>(itr->second.get());
    }

private:
    template <typename Component>
    ManagerContainer::iterator createManager()
    {
        ManagerPtr manager{new Manager<Component>(), [](void *p) {
                               delete static_cast<Manager<Component> *>(p);
                           }};

        return managers.emplace(
            perfectpixel::bedrock::typeID<Component>(), std::move(manager)).first;
    }

    ManagerContainer managers;
};
} // namespace pp::ecs
