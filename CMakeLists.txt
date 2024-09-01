cmake_minimum_required (VERSION 3.12)

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
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/backends")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/depends/stb")

##################################################
# MACROS
##################################################

MACRO(PP_LIBRARY LIB)
	set ("${LIB}_SRC_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/libs/${LIB}")
	file(GLOB_RECURSE "${LIB}_SRC_FILES" CONFIGURE_DEPENDS "${${LIB}_SRC_DIR}/*.cpp" "${${LIB}_SRC_DIR}/*.h")
	source_group(TREE ${${LIB}_SRC_DIR} FILES ${${LIB}_FILES})
	list(APPEND ${LIB}_FILES ${${LIB}_SRC_FILES})

	set ("${LIB}_INCLUDE_DIR" "${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB}")
	file(GLOB_RECURSE "${LIB}_INCLUDE_FILES" CONFIGURE_DEPENDS "${${LIB}_INCLUDE_DIR}/*.h")
	source_group(TREE ${${LIB}_INCLUDE_DIR} PREFIX "include" FILES ${${LIB}_INCLUDE_FILES})
	list(APPEND ${LIB}_FILES ${${LIB}_INCLUDE_FILES})

	set("${LIB}_EXTERNAL_FILES" ${ARGN})
	list(LENGTH ${LIB}_EXTERNAL_FILES EXTRA_COUNT)
	if (${EXTRA_COUNT} GREATER 0)
		source_group("external" FILES ${${LIB}_EXTERNAL_FILES})
		list(APPEND ${LIB}_FILES ${${LIB}_EXTERNAL_FILES})
	endif()
	add_library(${LIB} ${${LIB}_FILES})
	set_target_properties(${LIB} PROPERTIES FOLDER libs)

	if (MSVC)
		target_compile_options(${LIB} PRIVATE /W4 /WX)
	else()
		target_compile_options(${LIB} PRIVATE -Wall -Wextra -Werror)
	endif()

	list(APPEND PP_LIBRARIES ${LIB})
ENDMACRO(PP_LIBRARY)

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

list(APPEND IMGUI_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/imgui_tables.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/imgui_widgets.cpp"
)

list(APPEND IMGUI_BACKEND_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/backends/imgui_impl_opengl3.cpp"
)
if(WIN32)
	list(APPEND IMGUI_BACKEND_FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/depends/imgui-docking/backends/imgui_impl_win32.cpp"
	)
endif()

list(APPEND STB_IMAGE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/depends/stb/stb_image.h"
)

PP_LIBRARY(core ${IMGUI_FILES} ${STB_IMAGE_FILES})
PP_LIBRARY(ecs)
PP_LIBRARY(renderer ${IMGUI_BACKEND_FILES})
PP_LIBRARY(serialization)
PP_LIBRARY(bedrock)


##################################################
# TARGETS
##################################################
enable_testing()
add_executable(RunTests ${UNIT_TEST_FILES})
target_compile_options(RunTests PRIVATE -DGTEST_DONT_DEFINE_TEST=1)
add_test(
	NAME RunTests
	COMMAND RunTests
)

add_executable(Pong WIN32 "${CMAKE_CURRENT_SOURCE_DIR}/examples/pong/pong.cpp" )

if (MSVC)
	target_compile_options(Pong PRIVATE /W4 /WX)
else()
	target_compile_options(Pong PRIVATE -Wall -Wextra -Werror)
endif()

list(APPEND LIBRARIES_EXTERNAL libglew_shared yaml-cpp)

if(WIN32)
	list(APPEND LIBRARIES_SYSTEM opengl32)
endif()
if(LINUX)
	list(APPEND LIBRARIES_SYSTEM GL X11)
endif()

list(APPEND LIBRARIES_TEST gtest gtest_main)

add_custom_target(perfectpixel_libs DEPENDS ${PP_LIBRARIES})

set(PP_LIBRARIES_RUNTIME ${PP_LIBRARIES} ${LIBRARIES_EXTERNAL} ${LIBRARIES_SYSTEM})
add_custom_target(perfectpixel_runtime DEPENDS ${PP_LIBRARIES_ALL})

add_dependencies(Pong perfectpixel_runtime)
target_link_libraries(Pong ${PP_LIBRARIES_RUNTIME})
target_link_libraries(RunTests  ${PP_LIBRARIES} ${LIBRARIES_TEST})

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/dist"
		"$<TARGET_FILE_DIR:Pong>"
)

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"$<TARGET_FILE_DIR:libglew_shared>"
		"$<TARGET_FILE_DIR:Pong>"
)
	
set_property(TARGET Pong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
