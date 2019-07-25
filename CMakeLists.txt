cmake_minimum_required (VERSION 3.0)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "\nIn-Source build not supported. Please build in a subdirectory.\nYou might need to delete CMakeCache.txt")
endif()

project (PerfectPixel)

set (PerfectPixel_VERSION_MAJOR 0)
set (PerfectPixel_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 17)

add_definitions(-DPP_CLEANUP_CALLBACKS)
add_definitions(-DPP_FULL_REFLECTION)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##################################################
# SUBMODULES
##################################################
# SUBMODULE yaml-cpp 
set_property(GLOBAL PROPERTY FOLDER yaml-cpp)
add_subdirectory("${CMAKE_SOURCE_DIR}/depends/yaml-cpp" EXCLUDE_FROM_ALL)
set_target_properties(yaml-cpp PROPERTIES FOLDER depends/yaml-cpp)

# SUBMODULE zlib
add_subdirectory("${CMAKE_SOURCE_DIR}/depends/zlib" "${CMAKE_CURRENT_BINARY_DIR}/depends/zlib" EXCLUDE_FROM_ALL)
set_target_properties(zlib PROPERTIES FOLDER depends/zlib)

# SUBMODULE libpng
add_subdirectory("${CMAKE_SOURCE_DIR}/depends/libpng" "${CMAKE_CURRENT_BINARY_DIR}/depends/libpng" EXCLUDE_FROM_ALL)
add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/depends/libpng/pnglibconf.h
	COMMAND "${CMAKE_COMMAND}" -E copy
		"${CMAKE_SOURCE_DIR}/depends/libpng/scripts/pnglibconf.h.prebuilt"
		"${CMAKE_SOURCE_DIR}/depends/libpng/pnglibconf.h"
)
add_custom_target(pnglibconf ALL
	DEPENDS "${CMAKE_SOURCE_DIR}/depends/libpng/pnglibconf.h"
)
add_dependencies(png pnglibconf)

set_target_properties(png PROPERTIES FOLDER depends/libpng)
set_target_properties(genfiles PROPERTIES FOLDER depends/libpng)
set_target_properties(pnglibconf PROPERTIES FOLDER depends/libpng)

# SUBMODULE glew-cmake
add_subdirectory("${CMAKE_SOURCE_DIR}/depends/glew" "${CMAKE_CURRENT_BINARY_DIR}/depends/glew" EXCLUDE_FROM_ALL)
set_target_properties(libglew_shared PROPERTIES FOLDER depends/glew)
	
# INCLUDE PATHS
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/depends/yaml-cpp/include")
include_directories("${CMAKE_SOURCE_DIR}/depends/zlib")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/depends/zlib")
include_directories("${CMAKE_SOURCE_DIR}/depends/libpng")
include_directories("${CMAKE_SOURCE_DIR}/depends/glew/include")

##################################################
# MACROS
##################################################
MACRO(SOURCE_GROUP_WRAP GROUP SRC)
	foreach(FILE ${SRC})
		string(REPLACE "/" "\\" NGROUP "${GROUP}")
		source_group("${NGROUP}" FILES "${FILE}")
	endforeach()
ENDMACRO(SOURCE_GROUP_WRAP)

MACRO(MAKE_GROUP GNAME GFOLDER)
	set ("${GNAME}_SRC_DIR" "${CMAKE_SOURCE_DIR}/src/${GFOLDER}")
	set ("${GNAME}_INCLUDE_DIR" "${CMAKE_SOURCE_DIR}/include/${GFOLDER}")

	foreach(FILE ${ARGN})
		list(APPEND "${GNAME}_SRC_FILES" "${${GNAME}_SRC_DIR}/${FILE}")
	endforeach()
	
	file(GLOB "${GNAME}_INCLUDE_FILES" "${${GNAME}_INCLUDE_DIR}/*.h")
	
	SOURCE_GROUP_WRAP("Source Files/${GNAME}" "${${GNAME}_SRC_FILES}")
	SOURCE_GROUP_WRAP("Header Files/${GNAME}" "${${GNAME}_INCLUDE_FILES}")
	
	set ("${GNAME}_FILES" "${${GNAME}_SRC_FILES}" "${${GNAME}_INCLUDE_FILES}")
ENDMACRO(MAKE_GROUP)


MAKE_GROUP("Core" "enginecore"
	"Entry.cpp"
	"Game.cpp"
	"Win32Initializer.cpp"
)

MAKE_GROUP("Resources" "Resources"
	"Resource.cpp"
	"ResourceManager.cpp"
)

MAKE_GROUP("Input" "Input"
	"InputManager.cpp" 
)

MAKE_GROUP("Physics" "physics"
	"ColliderComponent.cpp" 
	"PhysicsComponent.cpp" 
	"CollisionProcessor.cpp" 
	"IntegratorProcessor.cpp"
)

MAKE_GROUP("Graphics" "graphics"
	"GraphicsManager.cpp" 
	"PNG.cpp" 
	"ShaderProgram.cpp" 
	"Sprite.cpp" 
	"SpriteComponent.cpp" 
	"Texture.cpp" 
	"VAO.cpp" 
	"FrameBuffer.cpp" 
	"windows/Win32Window.cpp" 
	"CBFGFont.cpp"
	"UIProcessor.cpp"
)

MAKE_GROUP("Bedrock" "Bedrock"
	"vectors.cpp" 
	"matrices.cpp" 
	"AARectangle.cpp" 
	"Circle.cpp" 
	"File.cpp" 
	"PpException.cpp"
	"BitSet.cpp"
	"Logger.cpp"
	"TypesSerialization.cpp"
)

MAKE_GROUP("EntityComponentSystem" "EntityComponentSystem"
	"Entity.cpp"
	"EntityManager.cpp" 
	"Query.cpp" 
	"Processor.cpp" 
	"ProcessorQueue.cpp"
)

MAKE_GROUP("Serialization" "serialization"
	"BinarySerializer.cpp"
	"ISerializer.cpp"
	"YAMLSerializer.cpp"
)

##################################################
# TARGETS
##################################################
add_library(LibPerfectPixel 
	"${Core_FILES}"
	"${Resources_FILES}" 
	"${Input_FILES}" 
	"${Physics_FILES}" 
	"${Graphics_FILES}" 
	"${Bedrock_FILES}" 
	"${EntityComponentSystem_FILES}" 
	"${Serialization_FILES}" 
)

add_executable(Pong WIN32 "${CMAKE_SOURCE_DIR}/src/main/pong.cpp")
add_dependencies(Pong yaml-cpp)

list(APPEND LIBRARIES_EXTERNAL png zlib libglew_shared yaml-cpp)
list(APPEND LIBRARIES_SYSTEM opengl32)

target_link_libraries(Pong LibPerfectPixel)
target_link_libraries(Pong ${LIBRARIES_EXTERNAL})
target_link_libraries(Pong ${LIBRARIES_SYSTEM})

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"${CMAKE_SOURCE_DIR}/dist"
		"$<TARGET_FILE_DIR:Pong>"
)

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"$<TARGET_FILE_DIR:png>"
		"$<TARGET_FILE_DIR:Pong>"
)

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"$<TARGET_FILE_DIR:zlib>"
		"$<TARGET_FILE_DIR:Pong>"
)

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"$<TARGET_FILE_DIR:libglew_shared>"
		"$<TARGET_FILE_DIR:Pong>"
)

set_property(TARGET Pong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
