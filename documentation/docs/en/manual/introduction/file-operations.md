# File Operations

ORF can open, move, rename, delete and compress the files you add to it.

## Overview

ORF is meant to work next to your system file manager, not replace it. Most commands are in the context menu: select one or more items and right-click (or long-press with a pen).

!!! warning "Operations change the original files"
    Move, rename and delete act on the real files on your drive, not on a copy. To protect your originals, use [Backup Mode or the ORF Folder](#protect-your-originals).

## Open files

- Double-click a file that has a preview, such as an image, to open it in [PureView](../plugins/pureview.md): the linked one, or a new window if there is none. See [Supported File Formats](supported-files.md).
- Double-click any other file to open it with its default application. With PureView turned off, all files open this way.
- Right-click and choose **Open With** to pick another application for this time. You can also make it the new default for that file type.

### Default applications

By default ORF uses the application your operating system has registered for the file type. To override it:

1. Open **Preferences > File Management > Default Applications**.
2. Choose a file extension, for example `.clip`.
3. Choose the application that should open it.

## Move, rename and delete

| Task | How |
| --- | --- |
| **Rename** | Select one item, then right-click > **Rename**. Type the new name and press `Enter`. |
| **Move** | Drag the selection onto a folder, or right-click > **Move to** and choose a destination. Dragging always moves, also to a folder on another drive. |
| **Copy** | Select the items, then press `Ctrl+C` (`Cmd+C`) or right-click > **Copy**. In the target folder, press `Ctrl+V` (`Cmd+V`) or right-click > **Paste**. |
| **Bulk move** | Select several items, then right-click > **Move to**. All selected items go to the same folder. |
| **Delete** | Right-click > **Move to Trash**. |

Delete sends files to your system trash, so you can restore them there.

## System context menu

- **Show in Explorer** (Windows), **Show in Finder** (macOS) and **Show in file manager** (Linux): right-click an item to open the system file manager with that item selected.
- **Show more options** (Windows only): right-click an item to open the Windows context menu for the selection. Its entries come from other apps, such as 7-Zip, WinRAR, NanaZip and Microsoft Defender.
- On Linux, **Show in file manager** needs a file manager that supports selecting a file. Nautilus, Dolphin, Nemo, Thunar and Caja do. Otherwise ORF offers **Open containing folder**, which opens the folder in your default folder app without selecting the file. If neither is available, the entry is not shown.
- The entry is not shown for virtual folders, which have no file on disk. In ORF Folder mode, it shows the copy inside the ORF Folder.

!!! note "ORF does not control these entries"
    The extra entries are added by other apps, and they run inside ORF while the menu is open. A faulty entry can make ORF stop responding. ORF loads the menu only when you choose **Show more options**.

For `.rar` and `.7z` files, use the entry of your archive app in this menu.

## Compress and extract

- Select items, then right-click > **Compress to .zip**.
- Select a `.zip` file, then right-click > **Extract**.

ORF supports `.zip`. Other archive formats such as `.rar` and `.7z` open with your system's applications.

## Protect your originals

Two optional modes keep your original files untouched. Choose one in **Preferences > General > File Operations**.

=== "Backup Mode"

    Before ORF changes a file, it saves a copy to a backup directory that you choose. Your files stay where they are, and ORF operates on them directly.

    Use it if you add folders from your drive and want a safety net.

=== "ORF Folder"

    ORF works only inside a dedicated folder. Files you add are copied into it, and ORF reads and changes only that folder.

    Use it if you want a clear boundary around the files ORF manages. It needs the [ORF Folder plugin](../plugins/orf-folder.md) and uses disk space for the copies.

## Related topics

- [Drag and Drop](drag-and-drop.md)
- [File Browser](file-browser.md)
- [ORF Folder](../plugins/orf-folder.md)
- [Preferences](../preferences/index.md)
