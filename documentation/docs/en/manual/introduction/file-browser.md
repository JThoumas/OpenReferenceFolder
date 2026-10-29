# File Browser

The file browser shows the contents of a folder as thumbnails or as a list.

## Overview

The file browser is on the right side of the [homepage](homepage.md). It starts with a navigation tree. Open a folder from the tree to browse it. The browser reads the folder from your drive, so it always shows the current contents. Hidden files and system clutter are left out. See [View Filtering](filtering.md).

## Navigation tree

The navigation tree works like the tree in a system file manager. The folders and virtual folders you added are its top-level entries. Expand an entry to see what is inside, and select one to browse it. To add folders to the tree, see [Homepage](homepage.md#add-folders-and-files).

## Navigate

- Double-click a folder to open it.
- Use the breadcrumb bar at the top to jump to any parent folder. Click a name in the path to go there.
- Use the back and forward buttons to move through the folders you visited.

## Choose a view

Switch views from the toolbar.

| View | Shows |
| --- | --- |
| **Grid** | Large thumbnails with file names. Use it for images. |
| **List** | One row per file with name, type, size and date. Use it for long folders. |

## Sort

Choose a sort key from the toolbar: name, type, size or date modified. Click the same key again to reverse the order.

Names are sorted in *natural order*: `image2.png` comes before `image10.png`.

!!! note
    PureView follows the order you see in the file browser. When you press the arrow keys in the viewer, the next image is the next one in your sorted list.

## Select

| Action | Result |
| --- | --- |
| Click | Select one item |
| `Ctrl` / `Cmd` + click | Add or remove an item from the selection |
| `Shift` + click | Select a range |
| `Ctrl` / `Cmd` + `A` | Select all |
| Drag on empty space | Select the items inside the rectangle |

Right-click a selection for the context menu. See [File Operations](file-operations.md).

## Thumbnails

ORF creates thumbnails in the background, so the window stays responsive while a large folder loads. Thumbnails near the part of the list you are looking at are created first.

ORF saves thumbnails in a cache so that they appear immediately the next time you open the folder. The cache is keyed on the file path, size and modification date, so a changed file gets a new thumbnail.

To see where the cache is or to clear it, see [Data Locations](../installation/data-locations.md).

## Related topics

- [View Filtering](filtering.md)
- [Supported File Formats](supported-files.md)
- [PureView](../plugins/pureview.md)
