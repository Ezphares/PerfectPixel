#pragma once

#include <typeinfo>
#include <sstream>

#include <Bedrock/Hash.h>
#include <Bedrock/PpException.h>
#include <Bedrock/numbers.h>
#include <Bedrock/defines.h>

#if PP_FULL_REFLECTION_ENABLED
#define PP_TYPENAME_IMPL(T, TSHORT) template <> inline std::string typeName<T>() { return #TSHORT; }
#else
#define PP_TYPENAME_IMPL(T, TSHORT)
#endif // PP_FULL_REFLECTION_ENABLED


#define PP_TYPE_REFLECTION(T, TSHORT) \
namespace perfectpixel { namespace bedrock { \
template <> inline int32_t typeID<T>() { return PP_ID(TSHORT); } \
PP_TYPENAME_IMPL(T, TSHORT) \
} }

namespace perfectpixel { namespace bedrock {

	template <typename T>
	int32_t typeID()
	{
		std::stringstream err;
		err << "No TypeID registered for requested type [" << typeid(T).name() << "]";

		throw PpException(err.str());
	}

#if PP_FULL_REFLECTION_ENABLED
	template <typename T>
	std::string typeName()
	{
		std::stringstream err;
		err << "No TypeName registered for requested type [" << typeid(T).name() << "]";

		throw PpException(err.str());
	}
#endif // PP_FULL_REFLECTION_ENABLED

} }

PP_TYPE_REFLECTION(int32_t, int32_t);
PP_TYPE_REFLECTION(float, float);
