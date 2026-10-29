# FAQ

Short answers to common questions. For problems that depend on your operating system, see [Platform Questions](platforms.md).

## General

### Is ORF free?

Yes. ORF is free and open source under the [AGPL-3.0 License](https://github.com/JThoumas/OpenReferenceFolder/blob/main/LICENSE). The logo is not covered by that license. See the disclaimer on the [homepage](../index.md#disclaimer).

### Does ORF copy my files?

No, not by default. ORF points to the files where they are. The exceptions are the [ORF Folder plugin](../manual/plugins/orf-folder.md), which copies files into its own folder, and images you drag in from a web browser, which have to be saved somewhere. An image dropped on the unlinked PureView tab is not saved by default. See [Drag and Drop](../manual/introduction/drag-and-drop.md).

### Does ORF change my files?

Only when you ask it to, with a command such as move, rename or delete. ORF's index never changes your files. To protect the originals, use Backup Mode or the ORF Folder. See [File Operations](../manual/introduction/file-operations.md#protect-your-originals).

### What happens if I move a file outside ORF?

ORF marks the entry as **missing**. It does not delete the entry. Select the entry and choose **Re-link** to point it to the new location. If you moved a whole folder, re-link the folder.

### What happens if the ORF database is damaged?

The database is an index, so your files are not lost. Delete it as described in [Data Locations](../manual/installation/data-locations.md#reset-orf) and add your folders again.

### Does ORF work offline?

Yes. ORF does not need an internet connection. The exceptions are the things you start yourself, such as dragging an image from a website.

### Does ORF update itself?

No. ORF does not update by default. An update check is optional and off until you turn it on. You can always download new versions from the [releases page](https://github.com/JThoumas/OpenReferenceFolder/releases).

### Can I use ORF with a pen tablet?

Yes. The pen works as a pointer. See [Pen Tablets](../manual/pen-tablets.md).

### Why does a file show a plain icon instead of a preview?

ORF has no decoder for that format. Built-in previews cover PNG, JPEG, BMP, TGA and GIF. A decoder plugin may add more. Any file can still be opened in your system's application. See [Supported File Formats](../manual/introduction/supported-files.md).

### Where does ORF save my settings?

See [Data Locations](../manual/installation/data-locations.md).

### How do I report a bug or suggest a feature?

Open an issue on [GitHub](https://github.com/JThoumas/OpenReferenceFolder/issues). Include your operating system and ORF version. See [Issues](../development/development-guide.md#issues) for what to include.

## Related topics

- [Getting Started](../manual/index.md)
- [Platform Questions](platforms.md)
- [Concepts](../manual/introduction/concepts.md)
