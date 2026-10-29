# Preferences

Preferences is where you change how ORF looks and behaves.

## Overview

Open Preferences from the top bar. The sidebar lists these sections:

- [General](#general)
- [Personalization](#personalization)
- [File Management](#file-management)
- [Compact Mode](#compact-mode)
- [Plugins](#plugins)

Changes apply immediately.

## General

| Setting | What it does |
| --- | --- |
| **Assets Folder** | The folder where ORF saves images dragged in from a browser when you do not drop them on a real folder. You choose it the first time you start ORF. If you skip the choice, it is `Pictures/ORF`. See [Drag and Drop](../introduction/drag-and-drop.md). |
| **Images dropped on PureView** | What ORF does with an image dropped on the unlinked [PureView](../plugins/pureview.md) tab. By default it is not saved and only held in the cache. You can change it to save into the assets folder or into a folder you choose. |
| **Startup Behavior** | Which folders or workspace open when ORF starts. |
| **File Operations** | Choose how ORF protects your originals: none, **Backup Mode**, or the **ORF Folder**. See [File Operations](../introduction/file-operations.md#protect-your-originals). |

## Personalization

| Setting | What it does |
| --- | --- |
| **Theme** | Light, dark, or follow your operating system. |
| **UI Presets** | Pick a built-in color scheme, or create your own and give it a name. |

## File Management

| Setting | What it does |
| --- | --- |
| **Default Applications** | Choose the application that opens a file extension, for example `.clip`. |
| **Custom Exclusions** | Extensions and folders that ORF always hides. See [View Filtering](../introduction/filtering.md). |

## Compact Mode

The settings for [Compact Mode](../introduction/compact-mode.md) are in their own tab. The ORF settings come first, then the settings of each valid plugin window or panel.

| Setting | What it does |
| --- | --- |
| **ORF windows and panels** | On by default. Turn it off to keep side and outer panels solid. |
| **Each plugin window or panel** | One entry for each valid plugin window or panel, listed under the ORF settings. It decides whether the plugin stays visible when the pointer leaves the ORF window. A plugin can offer more than on or off, for example its own opacity. |

## Plugins

Lists the plugins that ORF has. PureView, ZenRef and ORF Folder are built in. You can turn a built-in plugin off here, but you cannot uninstall it. Each plugin that has settings adds an entry to the sidebar. Plugin settings open in the same window, not in a separate one.

To add a plugin that is not built in:

1. Open **Preferences > Plugins**.
2. Choose **Install** and select a plugin zip file, or drag the zip file onto the page.
3. Restart ORF if the plugin asks for it.

Plugins that you installed can also be removed. See [Plugins Overview](../plugins/overview.md).

## Related topics

- [Compact Mode](../introduction/compact-mode.md)
- [Plugins Overview](../plugins/overview.md)
- [Data Locations](../installation/data-locations.md)
