cmake_minimum_required(VERSION 3.5)

project(photolab VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_NAME mlp)

add_subdirectory(lib/csv-parser)
include_directories(lib/csv-parser/include)

set(PROJECT_SOURCES
		main.cpp
		Matrix.hpp
		Network.hpp
		LinearAlloc.hpp
)

add_executable(${EXECUTABLE_NAME}
		${PROJECT_SOURCES}
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE csv)
