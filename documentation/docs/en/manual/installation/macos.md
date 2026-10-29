# macOS Installation Guide

!!! warning "Not released yet"
    ORF has no stable release yet. The install steps below apply from the first release. Until then, [build ORF from source](../../development/building.md).

## Requirements

1. Confirm supported platforms

    | Windows     | ==macOS==             | ==macOS==              | Linux         | 
    | ----------- | --------------------- | ---------------------- | ------------- |
    | Windows 11  | macOS 26 (Tahoe)      | macOS 12 (Monterey)    | Debian 10+    |
    | Windows 10  | macOS 15 (Sequoia)    | macOS 11 (Big Sur)     | Ubuntu 20.04+ |
    | Windows 8.1 | macOS 14 (Sonoma)     | macOS 10.15 (Catalina) | Fedora 32+    |
    | Windows 7   | macOS 13 (Ventura)    |                        | Arch Linux    |

2. **Graphics & Runtime**: OpenGL 3.3 core profile, which every supported Mac provides. No external runtimes are required.

## Installation

1. Download the [latest release](https://github.com/JThoumas/OpenReferenceFolder/releases)

    - ORF is a universal build for Apple silicon and Intel Macs.

2. Open the DMG and drag `OpenReferenceFolder.app` onto the Applications shortcut in the window

3. Open `OpenReferenceFolder.app` for the first time (see below)

### Open an unsigned app

ORF for macOS is **not signed or notarized**. Signing needs a paid Apple Developer membership, which the project does not have. macOS Gatekeeper therefore blocks the first launch.

1. Try to open `OpenReferenceFolder.app`. macOS shows a message that it cannot verify the app.
2. Open **System Settings > Privacy & Security**.
3. Scroll to **Security** and click **Open Anyway** next to ORF.
4. Confirm in the dialog that follows.

You only need to do this once. If macOS says the app is damaged, see the [platform FAQ](../../faq/platforms.md).

### Permissions

macOS asks for permission when ORF reads folders such as Documents, Desktop or Downloads, or external drives. Allow the folders you want to add. You can change this later in **System Settings > Privacy & Security > Files and Folders**.

## Post-Installation

- Head back to [Getting Started](../index.md) to continue your ORF setup.
