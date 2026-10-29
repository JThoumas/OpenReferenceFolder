# ORF Folder

The ORF Folder plugin gives ORF a dedicated folder on your drive and limits ORF to it. It is built in, so you can turn it on or off in **Preferences > Plugins** but you cannot uninstall it.

## Overview

Normally ORF points to files where they already are, and file operations act on the original files. With the ORF Folder plugin, ORF keeps its own copies in one folder and works only on those.

Use it when you want a clear boundary: what is in the ORF Folder is what ORF manages, and nothing else on your drive is touched.

!!! warning "Disk space"
    Files in the ORF Folder are copies. They use extra disk space.

## Set it up

1. Check that **ORF Folder** is turned on in **Preferences > Plugins**.
2. Open **Preferences > General > File Operations** and choose **ORF Folder**.
3. Choose the folder that ORF should use. Pick a folder on a drive with enough free space.

## What changes

- **Items already in ORF** are copied into the ORF Folder. Your original files are left where they were and are no longer used by ORF.
- **Items you add later** are copied into the folder as you add them.
- **File operations** (move, rename, delete) happen inside the ORF Folder only.
- **Browser drag-in** saves images into the ORF Folder.

Because your originals are not changed, you can return to the normal mode at any time. The copies stay in the ORF Folder until you delete them.

## ORF Folder or Backup Mode

| | ORF Folder | Backup Mode |
| --- | --- | --- |
| Originals | Not used after the copy | Used directly |
| Extra copies | Every file | Only files that ORF changes |
| Disk space | More | Less |

See [File Operations](../introduction/file-operations.md#protect-your-originals).

## Related topics

- [File Operations](../introduction/file-operations.md)
- [Preferences](../preferences/index.md)
- [Plugins Overview](overview.md)
