#pragma once

#include <sstream>
#include <typeinfo>

#include <Bedrock/Hash.h>
#include <Bedrock/PpException.h>
#include <Bedrock/defines.h>
#include <Bedrock/numbers.h>

#if PP_FULL_REFLECTION_ENABLED
#define PP_TYPENAME_IMPL(T, TSHORT)                                            \
    template <>                                                                \
    inline std::string perfectpixel::bedrock::typeName<T>()                    \
    {                                                                          \
        return #TSHORT;                                                        \
    }
#else
#define PP_TYPENAME_IMPL(T, TSHORT)
#endif // PP_FULL_REFLECTION_ENABLED

#define PP_TYPE_REFLECTION(T, TSHORT)                                          \
    template <>                                                                \
    inline int32_t perfectpixel::bedrock::typeID<T>()                          \
    {                                                                          \
        return PP_ID(TSHORT);                                                  \
    }                                                                          \
    PP_TYPENAME_IMPL(T, TSHORT)

namespace perfectpixel { namespace bedrock {

template <typename T>
int32_t typeID()
{
    std::stringstream err;
    err << "No TypeID registered for requested type [" << typeid(T).name()
        << "]";

    throw PpException(err.str());
}

template <>
inline int32_t typeID<std::nullptr_t>()
{
    return 0;
}

#if PP_FULL_REFLECTION_ENABLED
template <typename T>
std::string typeName()
{
    std::stringstream err;
    err << "No TypeName registered for requested type [" << typeid(T).name()
        << "]";

    throw PpException(err.str());
}

template <>
inline std::string typeName<std::nullptr_t>()
{
    return "nullptr_t";
}
#endif // PP_FULL_REFLECTION_ENABLED

}} // namespace perfectpixel::bedrock

PP_TYPE_REFLECTION(uint32_t, uint32_t);
PP_TYPE_REFLECTION(int32_t, int32_t);
PP_TYPE_REFLECTION(float, float);
PP_TYPE_REFLECTION(std::string, string);
PP_TYPE_REFLECTION(perfectpixel::bedrock::ID, ID)
