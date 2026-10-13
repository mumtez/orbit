cmake_minimum_required(VERSION 3.10)  # Or higher based on your version
project(Orbit)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set GLFW and OpenGL directories (optional if you installed via package manager)
find_package(OpenGL REQUIRED)

# Find the GLFW package (assuming it's installed via Homebrew or elsewhere)
find_package(glfw3 REQUIRED)

# If you installed it manually or it’s not detected, use a more specific path:
# find_library(GLFW_LIBRARY glfw3 PATHS /path/to/glfw)
# find_path(GLFW_INCLUDE_DIR glfw3.h PATHS /path/to/glfw/include)

# Add the include directories
include_directories(${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR})

# Define your source files
set(SOURCES src/main.cpp
        src/gravity.h)

# Create the executable
add_executable(Orbit ${SOURCES})

# Link OpenGL and GLFW
target_link_libraries(Orbit ${OPENGL_gl_LIBRARY} glfw)

# For macOS with OpenGL, you might need to link against specific frameworks
if(APPLE)
    target_link_libraries(Orbit "-framework OpenGL")
endif()
