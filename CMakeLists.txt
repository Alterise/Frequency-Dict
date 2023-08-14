set (CMAKE_CXX_STANDARD 20)

project(freq)

add_subdirectory(${PROJECT_SOURCE_DIR}/oneTBB)

include_directories(${PROJECT_SOURCE_DIR}/oneTBB/include)

add_executable(freq main.cpp)