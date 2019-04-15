#pragma once

#include <EntityComponentSystem/Entity.h>

#include <type_traits>
#include <string>

//////////////////////////////////////////////////////////////////////////
// This file contains a set of macro definitions for creating components
// for the entity-component system (new-style).
//
// The bare minimum is using "Component(N) { <struct body> };" for the 
// data definition followed by "FinalizeComponent(N);"
//
// If the component data need specialized constructors, assignment
// operators or destructors, use the provided macros, as the data
// part of the structure will have it's data mangled.
// If you MUST reference the data directly, use "ComponentDataName(N)"
//////////////////////////////////////////////////////////////////////////

namespace perfectpixel {
	namespace world {
		typedef int ComponentTypeId;
} }

#define ComponentDataName(N) PP_##N##_Data

#define COMPONENT_TYPE_NAME(N) #N
#define COMPONENT_TYPE_ID_GENERATOR(N) std::hash<std::string>()(#N)

#define Component(N) struct ComponentDataName(N)

#if defined(DEBUG) || defined(NDEBUG) || defined(_DEBUG)
#define COMPONENT_NAME_VARIABLE std::string m_componentName;
#define COMPONENT_NAME_INITIALIZER , m_componentName(getTypeName())
#else 
#define COMPONENT_NAME_VARIABLE
#define COMPONENT_NAME_INITIALIZER
#endif 

#define FinalizeComponent(N) struct N \
{ \
	inline static std::string getTypeName() { return COMPONENT_TYPE_NAME(N); } \
	inline static ::perfectpixel::world::ComponentTypeId getTypeId() { return COMPONENT_TYPE_ID_GENERATOR(N); } \
	\
	ComponentDataName(N) m_data; \
	perfectpixel::world::Entity m_entity; \
	COMPONENT_NAME_VARIABLE \
	\
	inline N(perfectpixel::world::Entity entity) \
		: m_data() \
		, m_entity(entity) \
		COMPONENT_NAME_INITIALIZER \
	{} \
};

#define ComponentData(Instance, Key) Instance ## .m_data. ## Key

#define ComponentConstructor(N) explicit ComponentDataName(N)
#define ComponentDefaultConstructor(N) ComponentDataName(N)()

#define ComponentCopyConstructor(N) ComponentDataName(N)(const ComponentDataName(N) &other)
#define ComponentMoveConstructor(N) ComponentDataName(N)(const ComponentDataName(N) &&other)
#define ComponentCopyAssignment(N) ComponentDataName(N) &operator=(const ComponentDataName(N) &other)
#define ComponentMoveAssignment(N) ComponentDataName(N) &operator=(const ComponentDataName(N) &&other)
#define ComponentDestructor(N) ~ComponentDataName(N)

