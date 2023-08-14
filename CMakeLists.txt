set (CMAKE_CXX_STANDARD 20)

project(freq)

include_directories(${PROJECT_SOURCE_DIR}/oneTBB/include)

add_executable(freq main.cpp)

target_link_libraries(freq tbb)
