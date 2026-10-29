# View Filtering

Filters hide the files you do not need so that a folder shows only your references.

## Overview

A folder often holds project files and reference images together with unrelated data such as `.exe` or `.txt` files. ORF applies filters to the view only. A filter never deletes or moves a file.

## Reference Filter

Turn on the **Reference Filter** in the filter bar to hide every file ORF cannot show as an image or project file. What remains are images and project formats that have a preview. See [Supported File Formats](supported-files.md).

Folders stay visible, so you can still navigate.

## Format isolation

To show one file type only:

1. Open the format drop-down in the filter bar.
2. Choose a format, for example `.clip`.
3. Only files with that extension are shown.

Choose **All formats** to clear it.

## Built-in exclusions

ORF always hides:

- files and folders your operating system marks as hidden,
- `.DS_Store`,
- `.git` folders.

## Custom ignore lists

To hide more, add your own rules.

1. Open **Preferences > File Management > Custom Exclusions**.
2. Add an extension (`.tmp`) or a folder name (`node_modules`).

Anything on the list is hidden everywhere in ORF.

## Related topics

- [File Browser](file-browser.md)
- [Supported File Formats](supported-files.md)
- [Preferences](../preferences/index.md)
