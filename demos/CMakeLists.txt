project(demos)
cmake_minimum_required(VERSION 3.2.2)

add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS -DIMGUI_IMPL_OPENGL_LOADER_GLEW )

find_package(PkgConfig)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

include_directories(.
                    ..
                    imgui)

file(GLOB SRC_LIST *.cpp
                   imgui/*.cpp
                   imgui/*.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} -lpthread)
