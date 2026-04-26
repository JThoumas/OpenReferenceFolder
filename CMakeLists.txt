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

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw freetype nlohmann_json)

# Build GLAD as a small static library
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

# stb_image for image loading
add_library(stb STATIC extern/stb/stb_image.cpp)
target_include_directories(stb PUBLIC extern/stb)

# SQLite3
add_library(sqlite3 STATIC extern/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC extern/sqlite)
target_compile_definitions(sqlite3 PRIVATE SQLITE_THREADSAFE=1)

# Framework library
add_library(orf_framework STATIC
    src/framework/core/framework.cpp
    src/framework/renderer/shader.cpp
    src/framework/renderer/renderer2d.cpp
    src/framework/renderer/font.cpp
    src/framework/widgets/widget.cpp
    src/framework/widgets/panel.cpp
    src/framework/widgets/label.cpp
    src/framework/widgets/button.cpp
    src/framework/widgets/scroll_view.cpp
    src/framework/widgets/context_menu.cpp
    src/framework/docking/dock_node.cpp
    src/framework/docking/split_node.cpp
    src/framework/docking/panel_node.cpp
    src/framework/docking/tab_group_node.cpp
    src/framework/docking/dock_manager.cpp
    src/framework/docking/floating_window.cpp
    src/app/model/database.cpp
    src/app/thumbnail/thumbnail_cache.cpp
    src/app/widgets/file_browser_widget.cpp
)
target_include_directories(orf_framework PUBLIC src)
target_link_libraries(orf_framework PUBLIC glad glfw freetype stb nlohmann_json::nlohmann_json sqlite3)

# Platform-specific system libraries & sources
if(APPLE)
    target_sources(orf_framework PRIVATE src/platform/macos/file_dialog.mm)
    target_link_libraries(orf_framework PUBLIC "-framework AppKit" "-framework Foundation")
elseif(WIN32)
    target_sources(orf_framework PRIVATE src/platform/windows/file_dialog.cpp)
else()
    target_sources(orf_framework PRIVATE src/platform/linux/file_dialog.cpp)
endif()

# Required on Linux/macOS for SQLite's dynamic loading internals
if(UNIX)
    target_link_libraries(sqlite3 PRIVATE dl pthread)
endif()

# Main executable
add_executable(OpenReferenceFolder 
    src/app/main.cpp
    src/app/application.cpp
)

target_include_directories(OpenReferenceFolder PRIVATE src)
target_link_libraries(OpenReferenceFolder PRIVATE orf_framework)

if (WIN32)
    target_link_libraries(OpenReferenceFolder PRIVATE opengl32)
elseif (APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(OpenReferenceFolder PRIVATE ${OPENGL_LIB})
else()
    target_link_libraries(OpenReferenceFolder PRIVATE GL)
endif()
