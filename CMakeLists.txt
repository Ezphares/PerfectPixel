cmake_minimum_required (VERSION 3.8)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	message(FATAL_ERROR "\nIn-Source build not supported. Please build in a subdirectory.\nYou might need to delete CMakeCache.txt")
endif()

project (PerfectPixel)

set (PerfectPixel_VERSION_MAJOR 0)
set (PerfectPixel_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 20)

add_definitions(-DPP_CLEANUP_CALLBACKS)
add_definitions(-DPP_FULL_REFLECTION)

# TODO: if windows
add_definitions(-DNOMINMAX)

#TODO zlib/libpng dependency fun
set(SKIP_INSTALL_ALL ON)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##################################################
# SUBMODULES
##################################################
# SUBMODULE yaml-cpp 
option(YAML_CPP_BUILD_TESTS "Enable testing" ON)
set (YAML_CPP_BUILD_TESTS OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/depends/yaml-cpp" EXCLUDE_FROM_ALL)
set_target_properties(yaml-cpp PROPERTIES FOLDER depends/yaml-cpp)

# SUBMODULE zlib
option(ZLIB_TESTS "Build zlib tests" ON)
set(ZLIB_TESTS OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/depends/zlib" "${CMAKE_CURRENT_BINARY_DIR}/depends/zlib" EXCLUDE_FROM_ALL)
set_target_properties(zlib PROPERTIES FOLDER depends/zlib)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/depends/zlib" "${CMAKE_CURRENT_BINARY_DIR}/depends/zlib")
set(ZLIB_LIBRARY zlib)

# SUBMODULE libpng
option(PNG_BUILD_ZLIB "Custom zlib Location, else find_package is used" OFF)
set(PNG_BUILD_ZLIB ON)
option(PNG_TESTS  "Build libpng tests" ON)
set(PNG_TESTS OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/depends/libpng" "${CMAKE_CURRENT_BINARY_DIR}/depends/libpng" EXCLUDE_FROM_ALL)
set_target_properties(png PROPERTIES FOLDER depends/libpng)
set_target_properties(genfiles PROPERTIES FOLDER depends/libpng)

# SUBMODULE glew-cmake
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/depends/glew" "${CMAKE_CURRENT_BINARY_DIR}/depends/glew" EXCLUDE_FROM_ALL)
set_target_properties(libglew_shared PROPERTIES FOLDER depends/glew)

# SUBMODULE googletest
option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  OFF)
set(gtest_force_shared_crt ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/depends/googletest" "${CMAKE_CURRENT_BINARY_DIR}/depends/googletest" EXCLUDE_FROM_ALL)
set_target_properties(gtest PROPERTIES FOLDER depends/googletest)
set_target_properties(gtest_main PROPERTIES FOLDER depends/googletest)
set_target_properties(gmock PROPERTIES FOLDER depends/googletest)
set_target_properties(gmock_main PROPERTIES FOLDER depends/googletest)

# INCLUDE PATHS
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/yaml-cpp/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/zlib")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/depends/zlib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/libpng")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/depends/libpng")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/glew/include")

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
	set ("${GNAME}_SRC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/src/${GFOLDER}")
	set ("${GNAME}_INCLUDE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/include/${GFOLDER}")

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
	"FileResourceLocator.cpp"
	"Template.cpp"
	
	"PNGImage.cpp"
	"Sprite.cpp" 
)

MAKE_GROUP("Input" "input"
	"InputManager.cpp" 
)

MAKE_GROUP("Physics" "physics"
	"ColliderComponent.cpp" 
	"PhysicsComponent.cpp" 
	"CollisionSystem.cpp" 
	"IntegratorSystem.cpp"
)

MAKE_GROUP("Graphics" "graphics"
	"GraphicsManager.cpp" 
	"ShaderProgram.cpp" 
	"SpriteComponent.cpp" 
	"Texture.cpp" 
	"VAO.cpp" 
	"FrameBuffer.cpp" 
	"windows/Win32Window.cpp" 
	"CBFGFont.cpp"
	"UISystem.cpp"
	"GUI.cpp"
	"UIShadow.cpp"
	"UILayoutTracker.cpp"
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
	"Quaternion.cpp"
)

MAKE_GROUP("EntityComponentSystem" "EntityComponentSystem"
	"Entity.cpp"
	"EntityManager.cpp" 
	"Query.cpp" 
	"QuerySystem.cpp" 
	"LifeCycleSystem.cpp" 
	"SystemQueue.cpp"
	"CoreComponentStorage.cpp"
	"Component.cpp"
	"ReflectionTable.cpp"
)

MAKE_GROUP("Serialization" "serialization"
	"BinarySerializer.cpp"
	"ISerializer.cpp"
	"YAMLSerializer.cpp"
)

list(APPEND UNIT_TEST_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_Vectors.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_Matrices.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_TransformedView.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_Quaternion.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_BitSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_TypeReflection.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_Entity.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_EntityManager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_CollisionSystem.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_UILayoutTracker.cpp"
	
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/testutil.h"
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

enable_testing()
add_executable(RunTests ${UNIT_TEST_FILES})
target_compile_options(RunTests PRIVATE -DGTEST_DONT_DEFINE_TEST=1)
add_test(
	NAME RunTests
	COMMAND RunTests
)

add_executable(Pong WIN32 "${CMAKE_CURRENT_SOURCE_DIR}/src/main/pong.cpp")


if (MSVC)
	target_compile_options(Pong PRIVATE /W4 /WX)
	target_compile_options(LibPerfectPixel PRIVATE /W4 /WX)
else()
	target_compile_options(Pong PRIVATE -Wall -Wextra -Werror)
	target_compile_options(LibPerfectPixel PRIVATE -Wall -Wextra -Werror)
endif()

list(APPEND LIBRARIES_EXTERNAL png zlib libglew_shared yaml-cpp)
list(APPEND LIBRARIES_SYSTEM opengl32)
list(APPEND LIBRARIES_TEST gtest gtest_main)

add_dependencies(LibPerfectPixel genfiles)
add_dependencies(Pong LibPerfectPixel ${LIBRARIES_EXTERNAL})

target_link_libraries(Pong LibPerfectPixel)
target_link_libraries(Pong ${LIBRARIES_EXTERNAL})
target_link_libraries(Pong ${LIBRARIES_SYSTEM})

target_link_libraries(RunTests LibPerfectPixel ${LIBRARIES_TEST})

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/dist"
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

# if (NOT SKIP_INSTALL_LIB AND NOT SKIP_INSTALL_ALL)
# install(EXPORT zlib
	# DESTINATION "${CMAKE_INSTALL_LIBDIR}")
# endif()
	
set_property(TARGET Pong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
