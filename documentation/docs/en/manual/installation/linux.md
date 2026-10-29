# Linux Installation Guide

!!! warning "Not released yet"
    ORF has no stable release yet. The install steps below apply from the first release. Until then, [build ORF from source](../../development/building.md).

## Requirements

1. Confirm supported platforms

    | Windows     | macOS                 | macOS                  | ==Linux==     | 
    | ----------- | --------------------- | ---------------------- | ------------- |
    | Windows 11  | macOS 26 (Tahoe)      | macOS 12 (Monterey)    | Debian 10+    |
    | Windows 10  | macOS 15 (Sequoia)    | macOS 11 (Big Sur)     | Ubuntu 20.04+ |
    | Windows 8.1 | macOS 14 (Sonoma)     | macOS 10.15 (Catalina) | Fedora 32+    |
    | Windows 7   | macOS 13 (Ventura)    |                        | Arch Linux    |

2. **Graphics & Runtime**: OpenGL 3.3 support (`libgl1` / Mesa). No external runtimes are required.

!!! note
    ORF runs on X11 and on Wayland. Wayland support is not tested on every desktop environment yet. See the [platform FAQ](../../faq/platforms.md).

## Installation

ORF is distributed as an **AppImage**.

1. Download the AppImage from the [latest release](https://github.com/JThoumas/OpenReferenceFolder/releases).
2. Make it executable:

    ```bash
    chmod +x OpenReferenceFolder-*.AppImage
    ```

3. Run it:

    ```bash
    ./OpenReferenceFolder-*.AppImage
    ```

If the AppImage does not start, your system may lack FUSE 2. Run it with `--appimage-extract-and-run`, or install FUSE 2 from your distribution's packages.

## Post-Installation

- Head back to [Getting Started](../index.md) to continue your ORF setup.
