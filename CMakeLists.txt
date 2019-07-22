cmake_minimum_required (VERSION 3.0)

project (PerfectPixel)

set (PerfectPixel_VERSION_MAJOR 0)
set (PerfectPixel_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 17)

add_definitions(-DPP_CLEANUP_CALLBACKS)
add_definitions(-DPP_FULL_REFLECTION)

# SUBMODULE yaml-cpp 
option(YAML_CPP_BUILD_TESTS "Enable testing" ON)
set(YAML_CPP_BUILD_TESTS OFF)
option(YAML_CPP_BUILD_TOOLS "Enable parse tools" ON)
set(YAML_CPP_BUILD_TOOLS OFF)
option(YAML_CPP_BUILD_CONTRIB "Enable contrib stuff in library" ON)
set(YAML_CPP_BUILD_CONTRIB OFF)
option(YAML_CPP_INSTALL "Enable generation of install target" ON)
set(YAML_CPP_INSTALL OFF)
add_subdirectory("${CMAKE_SOURCE_DIR}/depends/yaml-cpp")


# PACKAGES
find_package(ZLIB)
if (ZLIB_FOUND)
	include_directories(${ZLIB_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "No ZLIB found")
endif()

find_package(PNG)
if (PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "No PNG found")
endif()

find_package(GLEW)
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "No GLEW found")
endif()
	
# INCLUDE PATHS
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/depends/yaml-cpp/include")

# LIBRARIES
set (Core_SRC_DIR "${CMAKE_SOURCE_DIR}/src/enginecore")
set (Core_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/enginecore")
file (GLOB Core_INCLUDE_FILES "${Core_INCLUDE_DIR}/*.h")
add_library(Core 
	"${Core_SRC_DIR}/Entry.cpp" 
	"${Core_SRC_DIR}/Game.cpp" 
	"${Core_SRC_DIR}/Win32Initializer.cpp" 
	
	"${Core_INCLUDE_FILES}"
)

set (Input_SRC_DIR "${CMAKE_SOURCE_DIR}/src/input")
set (Input_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/input")
file (GLOB Input_INCLUDE_FILES "${Input_INCLUDE_DIR}/*.h")
add_library(Input 
	"${Input_SRC_DIR}/InputManager.cpp" 
	
	"${Input_INCLUDE_FILES}"
)

set (Physics_SRC_DIR "${CMAKE_SOURCE_DIR}/src/physics")
set (Physics_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/physics")
file (GLOB Physics_INCLUDE_FILES "${Physics_INCLUDE_DIR}/*.h")
add_library(Physics 
	"${Physics_SRC_DIR}/ColliderComponent.cpp" 
	"${Physics_SRC_DIR}/PhysicsComponent.cpp" 
	"${Physics_SRC_DIR}/CollisionProcessor.cpp" 
	"${Physics_SRC_DIR}/IntegratorProcessor.cpp" 
	
	"${Physics_INCLUDE_FILES}"
)

set (Graphics_SRC_DIR "${CMAKE_SOURCE_DIR}/src/graphics")
set (Graphics_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/graphics")
file (GLOB Graphics_INCLUDE_FILES "${Graphics_INCLUDE_DIR}/*.h")
add_library(Graphics 
	"${Graphics_SRC_DIR}/GraphicsManager.cpp" 
	"${Graphics_SRC_DIR}/PNG.cpp" 
	"${Graphics_SRC_DIR}/ShaderProgram.cpp" 
	"${Graphics_SRC_DIR}/Sprite.cpp" 
	"${Graphics_SRC_DIR}/SpriteComponent.cpp" 
	"${Graphics_SRC_DIR}/Texture.cpp" 
	"${Graphics_SRC_DIR}/VAO.cpp" 
	"${Graphics_SRC_DIR}/FrameBuffer.cpp" 
	"${Graphics_SRC_DIR}/windows/Win32Window.cpp" 
	"${Graphics_SRC_DIR}/CBFGFont.cpp"
	"${Graphics_SRC_DIR}/UIProcessor.cpp"
	
	"${Graphics_INCLUDE_FILES}"
)

set (Bedrock_SRC_DIR "${CMAKE_SOURCE_DIR}/src/Bedrock")
set (Bedrock_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/Bedrock")
file (GLOB Bedrock_INCLUDE_FILES "${Bedrock_INCLUDE_DIR}/*.h")
add_library(Bedrock 
	"${Bedrock_SRC_DIR}/vectors.cpp" 
	"${Bedrock_SRC_DIR}/matrices.cpp" 
	"${Bedrock_SRC_DIR}/AARectangle.cpp" 
	"${Bedrock_SRC_DIR}/Circle.cpp" 
	"${Bedrock_SRC_DIR}/File.cpp" 
	"${Bedrock_SRC_DIR}/PpException.cpp"
	"${Bedrock_SRC_DIR}/BitSet.cpp"
	"${Bedrock_SRC_DIR}/Logger.cpp"
	"${Bedrock_SRC_DIR}/TypesSerialization.cpp"
	
	"${Bedrock_INCLUDE_FILES}"
)

set (EntityComponentSystem_SRC_DIR "${CMAKE_SOURCE_DIR}/src/EntityComponentSystem")
set (EntityComponentSystem_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/EntityComponentSystem")
file (GLOB EntityComponentSystem_INCLUDE_FILES "${EntityComponentSystem_INCLUDE_DIR}/*.h")
add_library(EntityComponentSystem  
	"${EntityComponentSystem_SRC_DIR}/Entity.cpp"
	"${EntityComponentSystem_SRC_DIR}/EntityManager.cpp" 
	"${EntityComponentSystem_SRC_DIR}/Query.cpp" 
	"${EntityComponentSystem_SRC_DIR}/Processor.cpp" 
	"${EntityComponentSystem_SRC_DIR}/ProcessorQueue.cpp" 
	
	"${EntityComponentSystem_INCLUDE_FILES}"
)

set (Serialization_SRC_DIR "${CMAKE_SOURCE_DIR}/src/serialization")
set (Serialization_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/serialization")
file (GLOB Serialization_INCLUDE_FILES "${Serialization_INCLUDE_DIR}/*.h")
add_library(Serialization
	"${Serialization_SRC_DIR}/BinarySerializer.cpp"
	"${Serialization_SRC_DIR}/ISerializer.cpp"
	"${Serialization_SRC_DIR}/YAMLSerializer.cpp"
	
	"${Serialization_INCLUDE_FILES}"
)


add_executable(Pong WIN32 "${CMAKE_SOURCE_DIR}/src/main/pong.cpp")
add_dependencies(Pong yaml-cpp)

list(APPEND LIBRARIES_INTERNAL  
	Core
	Input
	Physics
	Graphics
	Bedrock
	EntityComponentSystem
	Serialization
)

list(APPEND LIBRARIES_EXTERNAL PNG::PNG ZLIB::ZLIB GLEW::GLEW yaml-cpp)
list(APPEND LIBRARIES_SYSTEM opengl32)

target_link_libraries(Pong ${LIBRARIES_INTERNAL})
target_link_libraries(Pong ${LIBRARIES_EXTERNAL})
target_link_libraries(Pong ${LIBRARIES_SYSTEM})

add_custom_command(TARGET Pong POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"${CMAKE_SOURCE_DIR}/dist"
		"$<TARGET_FILE_DIR:Pong>"
)

set_property(TARGET Pong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
