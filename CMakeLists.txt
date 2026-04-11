cmake_minimum_required(VERSION 3.20)
project(OpenReferenceFolder VERSION 0.1.0 LANGUAGES C CXX)

# C++17 is required - std::filesystem is used for file operations
set(CMAKE_CXX_STANDARD 17)          # Use C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce C++17 standard
set(CMAKE_CXX_EXTENSIONS OFF)       # Diables GNU extensions

# Fetch GLFW automatically using CMake's FetchContent module
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.9
)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Build GLAD as a small static library
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

# Main executable
add_executable(OpenReferenceFolder src/main.cpp)

target_link_libraries(OpenReferenceFolder PRIVATE glad glfw)

# Platform-specific system libraries
if (WIN32)
    target_link_libraries(OpenReferenceFolder PRIVATE opengl32)
elseif (APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(OpenReferenceFolder PRIVATE ${OPENGL_LIB})
else()
    target_link_libraries(OpenReferenceFolder PRIVATE GL)
endif()