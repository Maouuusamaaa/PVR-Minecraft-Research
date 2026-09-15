cmake_minimum_required(VERSION 3.22)
project(controlled_renderer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Reuse the exact EXP-001 bridge implementation; do not duplicate it.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../../../BRIDGE-IMPL ${CMAKE_CURRENT_BINARY_DIR}/bridge_impl)

add_library(controlled_renderer SHARED renderer.cpp)
target_link_libraries(controlled_renderer PRIVATE pvr_minecraft_bridge android log EGL GLESv2)
target_include_directories(controlled_renderer PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../../../BRIDGE-IMPL/include
)
