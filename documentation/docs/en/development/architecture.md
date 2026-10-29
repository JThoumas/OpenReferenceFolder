# Architecture

This page describes how the ORF source code is organized.

## Overview

ORF draws its own interface with OpenGL 3.3 core. It does not use a GUI toolkit. GLFW provides windows and input, and glad loads the OpenGL functions.

## Layers

The code is split into layers. A layer uses only the ones above it in this table.

| Layer | Folder | Contents | OpenGL? |
| --- | --- | --- | --- |
| **core** | `src/core` | Paths, settings, JSON, the data model, the database, the file-system service, caches, the dock tree | No |
| **gfx** | `src/gfx` | The 2D renderer, textures, fonts, icons | Yes |
| **ui** | `src/ui` | Widgets, layout, input, themes, commands, docking | Yes |
| **platform** | `src/platform/<os>` | File dialogs, drag and drop, trash, fonts and window details for each OS | OS APIs |
| **app** | `src/app` | The application, built from the layers above | Yes |

`core` has no OpenGL dependency, so unit tests can cover it on every system.

## Rendering

- **Event-driven.** ORF waits for events and draws a frame only when something changed. An idle ORF uses almost no CPU.
- **Batched.** The 2D renderer collects rectangles, rounded rectangles and textured quads into batches and clips with a scissor stack.
- **HiDPI.** Window size and framebuffer size are handled in one place, so mouse positions and drawing stay correct on high-resolution displays.
- **Fonts.** ORF uses the platform's own fonts and falls back to system fonts for scripts such as Japanese, Korean and Chinese. It does not bundle a font.

## Widget tree

Widgets form a retained tree. Each widget handles layout, painting and input, and marks itself dirty when it needs a redraw. The pen is treated as a pointer, so widgets follow the rules in [Pen Tablets](../manual/pen-tablets.md): large hit targets, long-press for context menus.

## Dock tree

Panel layout is a tree with three node types: **split** nodes divide space, **tab group** nodes hold tabs, and **panel** nodes hold the content. Floating windows are extra GLFW windows that share one OpenGL context. The tree is serialized to JSON for [workspaces](../manual/introduction/workspaces.md). A registry maps a panel type id to a factory, so a saved layout can recreate its panels, including plugin panels.

## Data model

The model has three node types: virtual folder, real folder reference and file reference. SQLite stores nodes and settings, with versioned migrations (`user_version`) and WAL. The database is an index that can be rebuilt. See [Concepts](../manual/introduction/concepts.md).

File-system work runs on worker threads: enumeration, filters, natural sort, and change watching with a debounce.

## Thumbnail pipeline

1. The browser requests thumbnails for the visible items.
2. A priority queue sends them to a worker pool. Requests for items that scroll out of view are cancelled.
3. A decoder reads the file. JPEG files use scaled decoding to reach thumbnail size quickly. Other formats go through stb_image, the built-in `.blend` preview reader, or a decoder plugin.
4. The main thread uploads the result to a GL texture. Only the main thread creates textures.
5. A memory-limited LRU cache holds textures. A disk cache in the OS cache directory keeps thumbnails between runs, keyed on path, size and modification time.

PureView uses a separate full-resolution pipeline with prefetch.

## App paths

An `AppPaths` component decides where data, config and cache go. It uses a `userdata/` folder next to the program when one exists, and the OS directories otherwise. It never uses the working directory. See [Data Locations](../manual/installation/data-locations.md).

## Platform integration

On Windows, the system context menu is built by the shell on demand (`SHCreateDefaultContextMenu`, `IContextMenu`) on the main thread. Shell extensions run in-process, so ORF loads the menu only when the user asks for it with **Show more options**.

Revealing a file in the system file manager uses a different call on each OS:

- Windows: `SHOpenFolderAndSelectItems`.
- macOS: `NSWorkspace activateFileViewerSelectingURLs:`.
- Linux: D-Bus `org.freedesktop.FileManager1.ShowItems`. If no service answers, ORF runs `xdg-open` on the parent folder when an app is registered for `inode/directory`. Otherwise the entry is hidden. The check runs each time the menu opens, and it needs a D-Bus client library (`libdbus-1`).

## Plugins

Plugins load through a C ABI. See [Plugin Development](plugin-development.md).

## Related topics

- [Building ORF](building.md)
- [Plugin Development](plugin-development.md)
- [Development Guide](development-guide.md)
