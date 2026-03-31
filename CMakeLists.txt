cmake_minimum_required(VERSION 3.20)
project(opengl-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Silence OpenGL deprecation warnings on macOS
if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

# =============================================================================
# FetchContent - Download dependencies at configure time
# =============================================================================
include(FetchContent)

# GLFW - Windowing library
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# GLM - Math library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)

# stb - Image loading (header-only)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)

# Make dependencies available
FetchContent_MakeAvailable(glfw glm stb)

# =============================================================================
# GLAD - Local files (OpenGL 4.1 Core)
# =============================================================================
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

# =============================================================================
# Main executable
# =============================================================================
add_executable(${PROJECT_NAME}
    main.cpp
    shader/shader.cpp
    camera/camera.cpp
    window/window.cpp
    model/model.cpp
    mesh/mesh.cpp
    material/material.cpp
    texture/texture.cpp
)

# =============================================================================
# Include directories
# =============================================================================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${stb_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external/tinyobjloader
)

# =============================================================================
# Link libraries (cross-platform)
# =============================================================================
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    glad
)

# Platform-specific OpenGL linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()
