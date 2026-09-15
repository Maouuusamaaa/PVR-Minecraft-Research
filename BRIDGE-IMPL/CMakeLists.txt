cmake_minimum_required(VERSION 3.22)
project(PVR_Minecraft_Bridge VERSION 0.1.0 LANGUAGES C CXX)

# ============================================================================
# PVR Minecraft Research — EXP-001 Native Bridge Implementation
#
# Objective: Build a minimal, controlled native library for Android ARM64
# that can be loaded into Minecraft Bedrock process.
#
# This is a research implementation ONLY. Does not contain:
# - RenderDragon hooks
# - BGFX interception
# - PVR renderer code
# - Any Minecraft proprietary libraries or binaries
#
# Contains ONLY:
# - Controlled initialization logging marker
# - Minimal logging infrastructure
# - Documentation on how to load and test
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Android-specific configuration
if(ANDROID)
    message(STATUS "[PVR-Bridge] Configuring for Android")
    message(STATUS "[PVR-Bridge] Android ABI: ${ANDROID_ABI}")
    message(STATUS "[PVR-Bridge] Android Platform: ${ANDROID_PLATFORM}")
    message(STATUS "[PVR-Bridge] Android STL: ${ANDROID_STL}")
else()
    message(WARNING "[PVR-Bridge] Not configured for Android. This bridge is intended for Android ARM64 only.")
endif()

# Ensure ARM64 target
if(NOT ANDROID_ABI MATCHES "arm64-v8a")
    message(WARNING "[PVR-Bridge] Expected ANDROID_ABI=arm64-v8a, got ${ANDROID_ABI}")
endif()

# ============================================================================
# Target: libpvr_minecraft_bridge.so
# ============================================================================

add_library(pvr_minecraft_bridge SHARED
    src/bridge_init.c
    src/bridge_log.c
)

target_include_directories(pvr_minecraft_bridge
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set shared library output
set_target_properties(pvr_minecraft_bridge PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    OUTPUT_NAME "pvr_minecraft_bridge"
)

# ============================================================================
# Android-specific linking
# ============================================================================
if(ANDROID)
    target_link_libraries(pvr_minecraft_bridge
        PRIVATE
            log  # Android native logging (liblog.so)
    )
endif()

# ============================================================================
# Optional: Testing infrastructure (build-time checks)
# ============================================================================
enable_testing()

# Verify header inclusion (compile-time check)
add_executable(check_headers_only
    EXCLUDE_FROM_ALL
    test/check_headers.c
)
target_include_directories(check_headers_only PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_test(
    NAME HeaderInclusionCheck
    COMMAND check_headers_only
)

# ============================================================================
# Installation
# ============================================================================
# Copy the built library to a known location for packaging/deployment
install(TARGETS pvr_minecraft_bridge
    LIBRARY DESTINATION lib/arm64-v8a
)

# ============================================================================
# Build Information
# ============================================================================
message(STATUS "")
message(STATUS "[PVR-Bridge] Build Configuration Summary:")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Target Library: libpvr_minecraft_bridge.so")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Android ABI: ${ANDROID_ABI}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared Library Output: ${CMAKE_CURRENT_BINARY_DIR}/libpvr_minecraft_bridge.so")
message(STATUS "")
message(STATUS "To build:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake .. -DCMAKE_ANDROID_ABI=arm64-v8a")
message(STATUS "  cmake --build .")
message(STATUS "")
