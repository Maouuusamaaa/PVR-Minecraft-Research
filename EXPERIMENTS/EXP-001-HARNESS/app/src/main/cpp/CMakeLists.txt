cmake_minimum_required(VERSION 3.22)
project(exp001_harness LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PVR_BRIDGE_LIBRARY
    "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/arm64-v8a/libpvr_minecraft_bridge.so")
if(NOT EXISTS "${PVR_BRIDGE_LIBRARY}")
    message(FATAL_ERROR "Missing verified libpvr_minecraft_bridge.so at ${PVR_BRIDGE_LIBRARY}")
endif()

add_library(pvr_minecraft_bridge SHARED IMPORTED GLOBAL)
set_target_properties(pvr_minecraft_bridge PROPERTIES
    IMPORTED_LOCATION "${PVR_BRIDGE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../../../../../../BRIDGE-IMPL/include"
)

add_library(exp001_harness SHARED harness_jni.cpp)
target_link_libraries(exp001_harness PRIVATE pvr_minecraft_bridge log)
