cmake_minimum_required(VERSION 3.12)

project(Benno)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BENNO_SRC_FILES
        "*.h"
        "*.cpp"
        )

include(../conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} ${BENNO_SRC_FILES})

include_directories(${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SG_DEBUG_BUILD)
endif()
