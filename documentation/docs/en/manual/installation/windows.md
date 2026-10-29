# Windows Installation Guide

!!! warning "Not released yet"
    ORF has no stable release yet. The install steps below apply from the first release. Until then, [build ORF from source](../../development/building.md).

## Requirements

1. Confirm supported platforms

    | ==Windows== | macOS                 | macOS                  | Linux         | 
    | ----------- | --------------------- | ---------------------- | ------------- |
    | Windows 11  | macOS 26 (Tahoe)      | macOS 12 (Monterey)    | Debian 10+    |
    | Windows 10  | macOS 15 (Sequoia)    | macOS 11 (Big Sur)     | Ubuntu 20.04+ |
    | Windows 8.1 | macOS 14 (Sonoma)     | macOS 10.15 (Catalina) | Fedora 32+    |
    | Windows 7   | macOS 13 (Ventura)    |                        | Arch Linux    |

2. **Graphics & Runtime**: OpenGL 3.3 capable GPU and drivers. No external runtimes are required.

## Installation

ORF for Windows is 64-bit (x64). Choose one of the two ways to install it.

=== "Installer"

    1. Download the Windows installer from the [latest release](https://github.com/JThoumas/OpenReferenceFolder/releases).
    2. Run the installer.
    3. Start ORF from the Start menu.

=== "Portable (zip)"

    1. Download the Windows zip file from the [latest release](https://github.com/JThoumas/OpenReferenceFolder/releases).
    2. Extract it to a folder you can write to.

        !!! warning
            Do not extract ORF to paths that need administrator (UAC) permission, such as `C:\` or `C:\Program Files\`. ORF cannot save its data there. See [Data Locations](data-locations.md).

    3. Create an empty folder named `userdata` next to `OpenReferenceFolder.exe`. See [Data Locations](data-locations.md#portable-mode).
    4. Run `OpenReferenceFolder.exe`.

!!! note "SmartScreen"
    Windows SmartScreen may warn about an application it does not know. See the [platform FAQ](../../faq/platforms.md).

## Post-Installation

- Head back to [Getting Started](../index.md) to continue your ORF setup.
- To keep ORF and its data together on a USB drive or in a folder, see [portable mode](data-locations.md).
