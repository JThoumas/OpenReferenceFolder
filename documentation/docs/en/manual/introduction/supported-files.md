# Supported File Formats

ORF shows any file your operating system can store. What differs is whether ORF can draw a preview.

## Overview

Formats fall into three tiers.

| Tier | What you get | How |
| --- | --- | --- |
| **1. Built-in previews** | Thumbnails and the PureView viewer | Included in ORF |
| **2. Plugin decoders** | Thumbnails and the viewer for more formats | Install a decoder plugin |
| **3. Everything else** | A file icon; opens with your system's application | Your OS |

## Tier 1: Built-in previews

| Format | Notes |
| --- | --- |
| `.png` | |
| `.jpg` / `.jpeg` | Large JPEGs are decoded at a reduced size for thumbnails, which is faster. |
| `.bmp` | |
| `.tga` | |
| `.gif` | The first frame only. |
| `.blend` | Shows the preview image that Blender stores in the file. |

These formats work without any plugin.

## Tier 2: Plugin decoders

Decoder plugins add previews for more formats. See [Plugins](../plugins/overview.md) for how to install them.

| Format | Status |
| --- | --- |
| `.svg` | First decoder plugin. Does not support the full SVG specification. |
| `.webp` | Planned |
| `.gif` (animated) | Planned |
| `.psd` / `.psb` | Planned. Shows the merged image of the file. |
| `.kra` | Planned. Shows the merged image of the file. |
| `.clip` | Planned. Shows the preview image stored in the file. |
| `.ai` | Later |

!!! note
    Decoders for project files show the preview image stored in the file. They do not read layers, and they do not edit the file.

## Tier 3: Everything else

Archives, video, documents, 3D files and all other types still appear in ORF, with a file icon. You can organize them and open them in your system's default application, or in the one you set in [File Operations](file-operations.md#default-applications).

| Examples | Notes |
| --- | --- |
| `.zip` | ORF can compress and extract this format. |
| `.rar`, `.7z` | Open with your system's application. |
| `.mp4`, `.webm` | No video preview. |
| `.pdf`, `.txt`, `.md` | No preview. |
| `.obj`, `.fbx` | No 3D preview. |

## Related topics

- [View Filtering](filtering.md)
- [Plugins Overview](../plugins/overview.md)
- [PureView](../plugins/pureview.md)
