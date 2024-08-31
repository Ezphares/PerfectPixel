#pragma once

#include "ecs/Manager.h"

#include "bedrock/Reflect.h"

#include <memory>
#include <unordered_map>

namespace pp::ecs {
class Context
{
    using ManagerPtr = std::unique_ptr<void, void (*)(void *)>;
    using ManagerContainer
        = std::unordered_map<perfectpixel::bedrock::TypeID, ManagerPtr>;

public:
    template <typename Component>
    Manager<Component> &getManager()
    {
        ManagerContainer::iterator itr
            = managers.find(perfectpixel::bedrock::Reflect<Component>::id());

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

        return managers
            .emplace(
                perfectpixel::bedrock::Reflect<Component>::id(),
                std::move(manager))
            .first;
    }

    ManagerContainer managers;
};
} // namespace pp::ecs
