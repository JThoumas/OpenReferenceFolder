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

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG        VER-2-13-2
)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw freetype)

# Build GLAD as a small static library
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

# stb_image for image loading
add_library(stb STATIC extern/stb/stb_image.cpp)
target_include_directories(stb PUBLIC extern/stb)

# Framework library
add_library(orf_framework STATIC
    src/framework/core/framework.cpp
    src/framework/renderer/shader.cpp
    src/framework/renderer/renderer2d.cpp
    src/framework/renderer/font.cpp
    src/framework/widgets/widget.cpp
)
target_include_directories(orf_framework PUBLIC src)
target_link_libraries(orf_framework PUBLIC glad glfw freetype stb)

# Main executable
add_executable(OpenReferenceFolder 
    src/app/main.cpp
    src/app/application.cpp
)

target_include_directories(OpenReferenceFolder PRIVATE src)
target_link_libraries(OpenReferenceFolder PRIVATE orf_framework)

# Platform-specific system libraries
if (WIN32)
    target_link_libraries(OpenReferenceFolder PRIVATE opengl32)
elseif (APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(OpenReferenceFolder PRIVATE ${OPENGL_LIB})
else()
    target_link_libraries(OpenReferenceFolder PRIVATE GL)
endif()