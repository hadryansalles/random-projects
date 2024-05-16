cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

# bin/lib output config
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

# set default output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# set default build types
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

# set build-specific output directories
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib/${OUTPUTCONFIG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin/${OUTPUTCONFIG})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

if(WIN32)
    if (CMAKE_GENERATOR MATCHES "Visual Studio")
        add_compile_options("/MP")
    endif()
endif()

# project config
project(cg)

# define CG_DEBUG if is debug build
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(CG_DEBUG)
endif()
# define CG_RELEASE if is release build
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    add_compile_definitions(CG_RELEASE)
endif()

# config relative lib folder
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib/${CMAKE_CFG_INTDIR})

# Project
file(GLOB_RECURSE GLSL_SOURCE "source/*.vert" "source/*.frag")
file(GLOB_RECURSE SOURCE "source/*.c" "source/*.h")
file(GLOB HEADERS "source/*")
add_executable(${PROJECT_NAME} ${SOURCE} ${GLSL_SOURCE})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE} ${GLSL_SOURCE})
include_directories(${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 23)

# glfw
add_subdirectory("deps/glfw")
include_directories("deps/glfw/include")
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# glad
include_directories("deps/glad")
file(GLOB_RECURSE GLAD_SOURCE "deps/glad/*.c")
add_library(glad ${GLAD_SOURCE})
target_link_libraries(${PROJECT_NAME} glad)

# other includes
include_directories("deps/")

# assets
if(UNIX)
    add_custom_target(run
        COMMAND ${CMAKE_SOURCE_DIR}/bin/${CMAKE_PROJECT_NAME}
        DEPENDS ${CMAKE_SOURCE_DIR}/bin/${CMAKE_PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} 
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    target_compile_options(cg PRIVATE "/Zi")
    target_compile_options(cg PRIVATE "/Ob0")
    target_compile_options(cg PRIVATE "/Od")
    target_compile_options(cg PRIVATE "/RTC1")
endif()

# log build types
message("-- CMake Build Type: ${CMAKE_BUILD_TYPE}")