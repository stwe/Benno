cmake_minimum_required(VERSION 3.17)

project(Benno)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${PROJECT_NAME}/src)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        Benno/src/App.cpp
        Benno/src/Log.h
        Benno/src/OpenGL.cpp
        Benno/src/OpenGL.h
        Benno/src/Window.cpp
        Benno/src/Window.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} libGL.so)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SG_DEBUG_BUILD)
endif()
