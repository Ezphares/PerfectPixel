#pragma once

#include <vector>
#include <iostream>

namespace pp::ecs {

template <typename Component>
struct Manager
{
    ~Manager()
    {
        std::cout << "Deleting..." << std::endl;
    }


private:
    std::vector<Component> components;
};
}

struct X
{
    int a;
    float b;
};

template <typename T>
T& access(X& v)
{
    return v.a;
}