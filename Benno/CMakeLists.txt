cmake_minimum_required(VERSION 3.17)

project(Benno)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BENNO_SRC_FILES
        "*.h"
        "*.cpp"
        )

add_executable(${PROJECT_NAME} ${BENNO_SRC_FILES})

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} libGL.so)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SG_DEBUG_BUILD)
endif()
