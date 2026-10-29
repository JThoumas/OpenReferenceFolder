# Plugin Development

This page describes how ORF plugins are built and loaded.

!!! warning "ABI v0: unstable"
    The plugin interface is at version 0. It will change without notice, and plugins built for one version may not load in another. Do not depend on it for released software yet.

## Overview

A plugin is a shared library with a manifest. ORF loads it through a small **C ABI**, so a plugin can be written in any language that can export C functions, and does not need the same compiler as ORF.

## The C ABI

The interface is declared in the header `orf_plugin.h`.

- **`struct_size`.** Every structure passed across the boundary starts with its size. ORF and the plugin use it to detect fields that the other side does not know about, so structures can grow without breaking older plugins.
- **API version window.** A plugin states the range of API versions it supports. ORF loads the plugin only if its own version is inside that range.
- **Host function table.** ORF gives the plugin a table of functions. A plugin calls ORF only through this table.

## The manifest

Each plugin has a `plugin.json` file.

| Field | Meaning |
| --- | --- |
| `id` | Unique identifier, for example `org.example.myplugin` |
| `version` | The plugin's own version |
| `license` | An [SPDX](https://spdx.org/licenses/) license identifier |
| `api` | The range of ORF API versions the plugin supports |
| `capabilities` | What the plugin provides |

## Capabilities

A plugin declares what it provides. ORF adds capabilities in this order:

1. **Decoders.** Turn a file into an image for thumbnails and the viewer.
2. **Panels, commands and settings.** Dockable panels, menu commands and an entry in Preferences.
3. **Windows.** Windows of their own.

A decoder-only plugin is the simplest kind and the first to be supported.

## Install location

Each plugin lives in its own folder in the per-user `plugins` directory inside the ORF [data directory](../manual/installation/data-locations.md). Users install a plugin from a zip file in **Preferences > Plugins**, or by copying a plugin folder into the plugin directory. They remove it in **Preferences > Plugins**.

## Safe mode

- `--no-plugins` starts ORF without loading any plugin. Use it to recover from a plugin that breaks startup.
- If a plugin fails to load, ORF turns it off automatically.

## License

ORF is licensed under the AGPL-3.0, and plugins run inside the ORF process. Whether a plugin must also be AGPL depends on how it links to ORF. This is not legal advice. Read the license and ask a lawyer if you plan to distribute a closed-source plugin. Declare your plugin's license in `plugin.json`.

## Related topics

- [Plugins Overview](../manual/plugins/overview.md)
- [Architecture](architecture.md)
- [Roadmap](roadmap.md)
