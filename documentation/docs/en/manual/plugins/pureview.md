# PureView

PureView shows an image in a panel or in its own window, so you can look at a reference without leaving ORF.

## Overview

PureView is a built-in plugin. It makes ORF work as a reference control panel: you pick files in the file browser, and PureView shows them. You can turn it off in **Preferences > Plugins**, but you cannot uninstall it.

Each PureView shows **one image**. To compare images, open more than one PureView. See [More than one PureView](#more-than-one-pureview).

## Unlinked PureView

The center of the [homepage](../introduction/homepage.md) is a PureView tab that is not linked to a file browser. It covers most of the window and works like a plain image viewer. An image dropped on it is not saved by default. ORF keeps it in the cache only. In **Preferences > General** you can change this to save it into the assets folder or into a folder you choose.

## Preview panel

| Action | Result |
| --- | --- |
| **Hover** over a file | The panel previews that file. |
| **Click** a file | Selects the file. When the pointer leaves, the panel returns to the file you selected last. |
| **Double-click** a file | Opens the file in the linked PureView, or in a new PureView window if there is none. |
| **Lock** button | Keeps the current image, even if you hover over or select other files. |

!!! warning "Window dependency"
    A PureView panel is linked to the ORF window it was created in. If you close that window, the panel closes with it. A floating viewer stays linked to its ORF window in the same way.

On a pen or touch screen, where there is no hover, tap a file to select it and show it.

## Full-resolution viewer

The panel shows a fitted preview. Open the viewer to see the image at full resolution. Press `Enter` or double-click the preview.

- Use the arrow keys to move to the next or previous image. The order is the one you see in the [file browser](../introduction/file-browser.md).
- ORF loads the neighbours of the current image in the background, so most steps show no wait. Very large images may still take a moment.
- Zoom with the scroll wheel or the zoom controls. Pan by dragging.

## Rotate and flip

| Control | Effect |
| --- | --- |
| **Rotate** | Turns the image in steps of 5°. |
| **Flip** | Mirrors the image horizontally or vertically. |

These controls change the view only. The file is not modified.

## More than one PureView

The tabs at the top of the window are pairs. Each pair tab holds one file browser and the PureView linked to it. To open another PureView, add another pair tab.

## Floating viewer

Drag the PureView tab out of the window to make an independent floating viewer. See [Docking Panels](../introduction/docking-panels.md).

- **Link border:** when you hover over or select a valid file in the linked file browser, the border of the floating window lights up.
- **Always on top:** keeps the viewer above your drawing software.
- **Borderless:** hides the title bar so the window looks like an image on your screen.
- **Background:** sets the color behind the image, for example neutral grey.

## Formats

PureView shows every format ORF can decode. See [Supported File Formats](../introduction/supported-files.md).

## Related topics

- [File Browser](../introduction/file-browser.md)
- [Docking Panels](../introduction/docking-panels.md)
- [Supported File Formats](../introduction/supported-files.md)
