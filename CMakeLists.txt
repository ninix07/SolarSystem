cmake_minimum_required(VERSION 3.10)

# Required version of cpp
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project("OpenGL_Lab" VERSION 0.1 LANGUAGES CXX C)

#find package
# find_package(Freetype REQUIRED)

message(STATUS "The build type is set as ${CMAKE_BUILD_TYPE}.")
# set build type as debug
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# create a list to keep source files in a place
set(SRCS "src/SolarSystem.cpp")

# append lists to add source files
# list(APPEND SRCS "src/src_1.cpp")

if (APPLE)
    # create bundle on apple compiles
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS} )
else()
    # the WIN32 is needed for Windows in order for it to look for WinMain
    # instead of the main function. This is ignored on other systems,
    # so it works on all platform
    add_executable(${PROJECT_NAME} ${SRCS})
endif()

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC include)

add_subdirectory("./lib")

target_link_libraries("${PROJECT_NAME}" ${OPENGL_LIBRARIES} glfw glad)
# target_link_libraries(demo ${FREETYPE_LIBRARIES})
# target_include_directories( "${PROJECT_NAME}" PRIVATE ${FREETYPE_INCLUDE_DIRS})


