# Roadmap

This page lists the planned work on ORF, in order.

## Overview

Work is split into milestones. The goals, in order, are the graphical interface, the file system, and plugins.

| Milestone | Goal | You can... |
| --- | --- | --- |
| **M1: GUI** | The interface | Open a themed window with docked, tabbed and floating panels that remember their layout |
| **M2: File system** | Files | Add folders, browse large image folders, work with files, and view images in PureView |
| **M3: Plugins** | Plugins | Turn the built-in plugins on and off, and install a plugin from a zip file |
| **Packaging** | Releases | Download a DMG, an AppImage or an installer and opt in to update checks |

## M1: GUI

- A window with an OpenGL 3.3 core context on Windows, macOS and Linux.
- A 2D renderer, text with native fonts, and icons.
- Widgets, themes and pen-friendly input.
- Docking: fixed regions and tabs, then dock guides and snapping, then floating windows.
- Workspaces: saved layouts, named workspaces and recovery.
- [Compact mode](../manual/introduction/compact-mode.md): side and outer panels overlay the inner panels and become solid on hover.

## M2: File system

- The data model and index.
- The [homepage](../manual/introduction/homepage.md) and [file browser](../manual/introduction/file-browser.md), with a navigation tree and thumbnails.
- [File operations](../manual/introduction/file-operations.md) and preferences.
- Drag and drop in and out of ORF.
- Show in Explorer/Finder and, on Windows, the system context menu (**Show more options**).
- [PureView](../manual/plugins/pureview.md) as a built-in plugin.
- Built-in `.blend` previews, which show the preview image stored in the file.

!!! note "Required for the first release"
    Dragging images in from a web browser must work on Windows, macOS and Linux before the first release.

## M3: Plugins

- The plugin interface and the loader. See [Plugin Development](plugin-development.md).
- Decoders: SVG first, then WebP, GIF animation, PSD/PSB, `.kra` and `.clip` previews. `.ai` comes later.
- [ORF Folder](../manual/plugins/orf-folder.md) and [ZenRef](../manual/plugins/zenref.md).
- Color Picker. It has no documentation page yet.
- A local first release: the built-in toggles and install from a zip file. An online plugin catalogue follows later.

## Packaging

Packaging starts after ORF works.

- A macOS DMG with an unsigned app, an Applications shortcut and a Gatekeeper note, a Linux AppImage and a Windows installer.
- A `NOTICES` file with third-party credits in every release.
- Draft releases with checksums.
- An opt-in update check.

## Not planned yet

- Pen pressure and tilt, and native pen APIs.
- Out-of-process plugins.
- Windows on Arm.
- Color management with ICC profiles, and complex-script text shaping.
- `.rar` and `.7z` archives, video and 3D previews.
- Signing and notarizing the macOS app.
- A macOS **Services** submenu in the context menu.

## Related topics

- [Development Guide](development-guide.md)
- [Architecture](architecture.md)
- [Getting Started](../manual/index.md)
