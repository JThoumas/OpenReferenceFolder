# Building ORF

This page shows how to build OpenReferenceFolder from source.

## Overview

ORF uses CMake and C++17. CMake downloads most dependencies for you with `FetchContent`, so the first configure needs an internet connection.

## Requirements

- **CMake 3.20 or newer**
- **A C++17 compiler**: MSVC 2019+, Clang 12+ or GCC 10+
- **OpenGL development files** for your system
- **Git**

### Linux packages

=== "Debian / Ubuntu"

    ```bash
    sudo apt-get install build-essential cmake git libgl1-mesa-dev \
      libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
      libwayland-dev libxkbcommon-dev wayland-protocols
    ```

=== "Other distributions"

    Install the equivalents of the packages above: OpenGL (Mesa) headers, X11 and XRandR/Xinerama/Xcursor/Xi development files, and the Wayland, xkbcommon and wayland-protocols development files.

GLFW builds both the Wayland and the X11 backends by default. To build for X11 only, configure with `-DGLFW_BUILD_WAYLAND=OFF` and skip the Wayland packages.

When native file dialogs are included in your build, Linux also needs GTK 3 (`libgtk-3-dev`) or the desktop portal.

## Build

1. Clone the repository:

    ```bash
    git clone https://github.com/JThoumas/OpenReferenceFolder.git
    cd OpenReferenceFolder
    ```

2. Configure:

    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    ```

3. Build:

    ```bash
    cmake --build build --config Release
    ```

4. Run the program from the `build` folder. On Windows with a multi-config generator, it is in `build\Release`.

For a universal macOS build, add `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"` to the configure step.

## Dependencies

`FetchContent` downloads GLFW, nlohmann/json and, when tests are on, Catch2. Other third-party code ships in the `extern/` folder. See the list on the [homepage](../index.md#acknowledgements).

## Tests

Tests are off by default. To build and run them:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DORF_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Tests use [Catch2](https://github.com/catchorg/Catch2) and cover the part of ORF that does not need OpenGL. See [Development Guide](development-guide.md).

## Continuous integration

Every push and pull request builds and tests ORF on three systems:

| System | Runner | Notes |
| --- | --- | --- |
| Windows | `windows-latest` | x64 |
| macOS | `macos-latest` | Universal (`arm64;x86_64`) |
| Linux | `ubuntu-latest` | Installs the packages listed above |

Releases are not published by CI yet.

## Troubleshooting

- **macOS:** If the window does not open, check that your Mac supports an OpenGL 3.3 core profile. ORF requests a forward-compatible core context.
- **Linux:** If CMake cannot find GLFW's dependencies, install the packages listed above.
- **Any system:** If `FetchContent` fails, check your internet connection and run the configure step again.

## Related topics

- [Architecture](architecture.md)
- [Development Guide](development-guide.md)
