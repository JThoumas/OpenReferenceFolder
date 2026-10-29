# Concepts

This page explains the few ideas that the rest of the manual builds on.

## Overview

ORF shows your files through an index. The index decides what appears in ORF. It does not hold your files.

## Virtual folders, real folders and files

ORF has three kinds of items.

| Item | What it is | Lives on disk? |
| --- | --- | --- |
| **Virtual folder** | A group that exists only in ORF. You use it to collect and organize items. | No |
| **Real folder** | A reference to a folder on your drive. ORF shows what is inside it. | Yes |
| **File** | A reference to a file on your drive. | Yes |

A virtual folder can contain real folders, files and other virtual folders. Because it has no path on disk, it does not take disk space, and deleting one does not delete any file.

The same file can appear in more than one virtual folder. All of them point to the one file on your drive.

## The database is an index

ORF keeps its list of items in a local database. Treat it as an index:

- The index stores *where* your files are and how you grouped them. It does not store your files.
- ORF never changes your original files to keep the index up to date.
- If the index is lost or damaged, you can rebuild it by adding your folders again. Your files are not affected.
- If a file is moved or deleted outside ORF, its entry is marked as **missing**. You can point the entry to the new location. See the [FAQ](../../faq/index.md#what-happens-if-i-move-a-file-outside-orf).

## The assets folder

When you drag an image in from a web browser, ORF saves it to one of these places:

1. The folder you are viewing, if you dropped onto a real folder.
2. Otherwise, the **assets folder**.

You choose the assets folder the first time you start ORF. If you skip the choice, it is `Pictures/ORF`. You can change it in [Preferences](../preferences/index.md). The assets folder is a plain directory outside the ORF data directory. It has no tags or sub-folders of its own, and you can open it in your system file manager.

An image dropped on the unlinked PureView tab is not saved by default. ORF keeps it in the cache only. You can change this in Preferences. See [Drag and Drop](drag-and-drop.md).

## Workspaces

A workspace is a saved arrangement of panels and windows. ORF saves your current arrangement automatically, and you can save named workspaces for different tasks. See [Workspaces](workspaces.md).

## Related topics

- [Homepage](homepage.md)
- [File Operations](file-operations.md)
- [Data Locations](../installation/data-locations.md)
