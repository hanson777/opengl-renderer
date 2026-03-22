cmake_minimum_required(VERSION 3.20)
project(opengl-renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Sources
add_executable(${PROJECT_NAME}
  main.cpp
  shader/shader.cpp
  camera/camera.cpp
  window/window.cpp
  model/mesh/mesh.cpp
  model/model.cpp
    ~/graphics/libs/glad/src/glad.c
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    /opt/homebrew/opt/glfw/include
    /opt/homebrew/opt/glm/include
    ~/graphics/libs/glad/include
    ~/graphics/libs
)

# Link
target_link_directories(${PROJECT_NAME} PRIVATE
    /opt/homebrew/opt/glfw/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)
