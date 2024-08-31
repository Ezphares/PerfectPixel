#pragma once

#include <sstream>
#include <typeinfo>

#include "bedrock/Hash.h"
#include "bedrock/defines.h"
#include "bedrock/numbers.h"

template <typename T>
constexpr bool Specialized = false;

#if PP_FULL_REFLECTION_ENABLED
#define _PP_DEFINE_NAME(T)                                                     \
    static constexpr const char *name()                                        \
    {                                                                          \
        return #T;                                                             \
    }
#else
#define _PP_DEFINE_NAME(T)
#endif // PP_FULL_REFLECTION_ENABLED

namespace perfectpixel::bedrock {

using TypeID = uint32_t;

template <typename T>
struct _Reflect
{
    static_assert(
        Specialized<T>,
        "No reflection information found for type. Did you forget PP_REFLECT?");

    static constexpr perfectpixel::bedrock::TypeID id()
    {
        return ~0;
    }
    _PP_DEFINE_NAME(T);
};

template <>
struct _Reflect<void>
{
    static constexpr perfectpixel::bedrock::TypeID id()
    {
        return 0;
    }
    _PP_DEFINE_NAME(void);
};

template <typename T>
using Reflect = _Reflect<std::remove_cvref_t<T>>;
} // namespace perfectpixel::bedrock

#define PP_REFLECT(T)                                                          \
    template <>                                                                \
    struct perfectpixel::bedrock::_Reflect<T>                                  \
    {                                                                          \
        static constexpr perfectpixel::bedrock::TypeID id()                    \
        {                                                                      \
            return PP_ID(T).m_hash;                                            \
        }                                                                      \
        _PP_DEFINE_NAME(T);                                                    \
    };

PP_REFLECT(std::nullptr_t);
PP_REFLECT(uint32_t);
PP_REFLECT(int32_t);
PP_REFLECT(float);
PP_REFLECT(std::string);
PP_REFLECT(perfectpixel::bedrock::ID);
