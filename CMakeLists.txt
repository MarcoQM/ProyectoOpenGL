cmake_minimum_required(VERSION 3.5)

project(ProyectoFinalCG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB files "*.h" "*.cpp") # files es nombre

add_executable(ProyectoFinalCG main.cpp ${files})

INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}  ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME}  glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME}  ${GLEW_LIBRARIES})
