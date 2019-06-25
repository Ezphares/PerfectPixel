cmake_minimum_required (VERSION 3.0)
project (PerfectPixel)

set (PerfectPixel_VERSION_MAJOR 0)
set (PerfectPixel_VERSION_MINOR 1)

set (CMAKE_CXX_STANDARD 17)

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

# LIBRARIES
set (Behaviour_SRC_DIR "${CMAKE_SOURCE_DIR}/src/behaviour")
set (Behaviour_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/behaviour")
file (GLOB Behaviour_INCLUDE_FILES "${Behaviour_INCLUDE_DIR}/*.h")
add_library(Behaviour 
	"${Behaviour_SRC_DIR}/BehaviourManager.cpp"
	
	"${Behaviour_INCLUDE_FILES}"
)

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
	"${Physics_SRC_DIR}/IntegratorProcessor.cpp.cpp" 
	
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
	
	"${Graphics_INCLUDE_FILES}"
)

set (Types_SRC_DIR "${CMAKE_SOURCE_DIR}/src/types")
set (Types_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/types")
file (GLOB Types_INCLUDE_FILES "${Types_INCLUDE_DIR}/*.h")
add_library(Types 
	"${Types_SRC_DIR}/vectors.cpp" 
	"${Types_SRC_DIR}/matrices.cpp" 
	"${Types_SRC_DIR}/AARectangle.cpp" 
	"${Types_SRC_DIR}/Circle.cpp" 
	"${Types_SRC_DIR}/File.cpp" 
	"${Types_SRC_DIR}/PpException.cpp"
	"${Types_SRC_DIR}/BitSet.cpp"
	"${Types_SRC_DIR}/Logger.cpp"
	
	"${Types_INCLUDE_FILES}"
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

set (Interface_SRC_DIR "${CMAKE_SOURCE_DIR}/src/managerinterface")
set (Interface_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/interface")
file (GLOB Interface_INCLUDE_FILES "${Interface_INCLUDE_DIR}/*.h")
add_library(Interface
	"${Interface_SRC_DIR}/ManagerInterface.cpp"
	
	"${Interface_INCLUDE_FILES}"
)

# set (Serialization_SRC_DIR "${CMAKE_SOURCE_DIR}/src/serialization")
# set (Serialization_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/serialization")
# file (GLOB Serialization_INCLUDE_FILES "${Serialization_INCLUDE_DIR}/*.h")
# add_library(Serialization
	# "${Serialization_SRC_DIR}/BinarySerializer.cpp"
	
	# "${Serialization_INCLUDE_FILES}"
# )

add_executable(Pong WIN32 "${CMAKE_SOURCE_DIR}/src/main/pong.cpp")

list(APPEND LIBRARIES_INTERNAL 
	Behaviour 
	Core
	Interface
	Input
	Physics
	Graphics
	Types
	EntityComponentSystem
)

list(APPEND LIBRARIES_EXTERNAL PNG::PNG ZLIB::ZLIB GLEW::GLEW)
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
