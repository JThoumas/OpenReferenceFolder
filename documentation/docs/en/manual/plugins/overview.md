# Plugins

Plugins are optional components that add features to ORF.

## Overview

ORF itself is a file manager. Plugins add the rest: more image formats, previews, and reference tools. PureView, ZenRef and ORF Folder are built in. They are listed in **Preferences > Plugins**, and you can turn them off but not uninstall them. A plugin can add:

- **Decoders**, which let ORF draw previews for more file formats.
- **Panels**, which you dock like any other panel.
- **Commands and settings**, which appear in menus and in [Preferences](../preferences/index.md).
- **Windows**, which are separate from the main window.

## Built-in plugins

<div class="grid cards" markdown>

-   **PureView**

    ---

    A preview panel and a full-resolution image viewer.

    [:octicons-arrow-right-24: PureView](pureview.md)

-   **ZenRef**

    ---

    Infinite canvases for your references, and a photo viewer.

    [:octicons-arrow-right-24: ZenRef](zenref.md)

-   **ORF Folder**

    ---

    A dedicated folder that ORF manages on its own.

    [:octicons-arrow-right-24: ORF Folder](orf-folder.md)

</div>

Decoder plugins for more file formats are listed in [Supported File Formats](../introduction/supported-files.md). The [Plugins](../../plugins/index.md) section has the full catalogue.

## Turn plugins on and off

1. Open **Preferences > Plugins**.
2. Use the switch of a plugin to turn it on or off.

You can turn off every plugin, including the built-in ones. If a plugin fails to load, ORF turns it off and keeps starting.

## Install and remove plugins

Built-in plugins need no install step, and they cannot be removed. To add another plugin:

1. Open **Preferences > Plugins**.
2. Choose **Install** and select a plugin zip file, or drag the zip file onto **Preferences > Plugins**.
3. To remove a plugin that you installed, select it and choose **Remove**. This deletes the plugin's folder.

Each plugin gets its own folder in the `plugins` folder of the [data directory](../installation/data-locations.md). Installing a newer zip of the same plugin updates it. You can also copy an unpacked plugin folder into the `plugins` folder and restart ORF.

The first plugin release is local: the built-in toggles and install from a zip file. An online list of plugins follows later.

!!! note
    Plugins run inside ORF with the same access to your files as ORF has. Install plugins only from sources you trust. To start ORF with all plugins off, run it with `--no-plugins`.

## Write a plugin

See [Plugin Development](../../development/plugin-development.md).

## Related topics

- [Preferences](../preferences/index.md)
- [Supported File Formats](../introduction/supported-files.md)
- [Compact Mode](../introduction/compact-mode.md)
