# Drag and Drop

You can drag files into ORF from other applications, and out of ORF into your drawing software.

## Overview

| Direction | From | To |
| --- | --- | --- |
| **Drag in** | System file manager, web browser | An ORF window |
| **Drag out** | ORF | A drawing application, a canvas or another folder |

## Drag files in

Drag files or folders from your system file manager onto an ORF window.

- Dropped on the navigation tree of the [file browser](file-browser.md), they are added to ORF as references. Nothing is copied.
- Dropped on a real folder you are viewing, they are moved into that folder.
- Dropped on the PureView tab, they open in the viewer. See [PureView](../plugins/pureview.md).

## Drag images in from a browser

You can drag an image from a web page into ORF. ORF has to save the image as a file, and it chooses the location like this:

1. **Contextual directory.** If you drop it on a real folder that you are viewing, ORF saves it into that folder on your drive.
2. **Assets folder.** If you drop it on the navigation tree or somewhere else that is not a real folder, ORF saves it to the assets folder. You choose it the first time you start ORF, and it is `Pictures/ORF` if you skip the choice. You can change it in **Preferences > General**.

An image dropped on the unlinked PureView tab is not saved by default. ORF keeps it in the cache only. In **Preferences > General** you can change this to save it into the assets folder or into a folder you choose.

Browsers and operating systems hand the image to ORF in different ways:

| OS | How the image arrives |
| --- | --- |
| **Windows** | As a file or a link, depending on the browser and the site. |
| **macOS** | As a file promise or a link. The browser writes the file when you drop it. |
| **Linux** | As a URI list through X11 or Wayland drag and drop. |

!!! note
    Some sites block image dragging or serve images that the browser cannot export as a file. If a drop does nothing, save the image from the browser and drag the saved file.

## Drag files out

Drag one or more files from the file browser onto another application, for example a canvas in your drawing software. The application receives the original file.

Dragging out does not move the file. To move a file inside ORF, drag it onto a folder in ORF. To copy a file inside ORF, use Copy and Paste. See [File Operations](file-operations.md).

## Related topics

- [Concepts](concepts.md)
- [File Operations](file-operations.md)
- [Preferences](../preferences/index.md)
