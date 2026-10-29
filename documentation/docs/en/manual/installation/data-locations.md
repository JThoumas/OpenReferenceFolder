# Data Locations

ORF stores its data either next to the program (portable) or in your user directories.

## Overview

ORF keeps three kinds of data:

| Kind | Contents |
| --- | --- |
| **Data** | The index database and installed plugins |
| **Config** | Preferences, themes, workspaces and layouts |
| **Cache** | Thumbnails. Safe to delete; ORF recreates them. |

ORF never writes to its working directory. It decides where to store data when it starts.

## Portable mode

If a folder named `userdata` exists next to the ORF program, ORF stores everything in it, including the cache. Nothing is written elsewhere.

1. Create an empty folder named `userdata` next to the ORF executable.
2. Start ORF.

Use portable mode to run ORF from a USB drive, or to keep several separate copies of ORF.

!!! warning
    Do not put ORF in a folder that needs administrator permission, such as `C:\Program Files\` on Windows. ORF cannot create `userdata` there.

## Standard locations

Without a `userdata` folder, ORF uses the standard directories of your system.

| | Data | Config | Cache |
| --- | --- | --- | --- |
| **Windows** | `%APPDATA%\ORF` | `%APPDATA%\ORF` | `%LOCALAPPDATA%\ORF\cache` |
| **macOS** | `~/Library/Application Support/ORF` | `~/Library/Application Support/ORF` | `~/Library/Caches/ORF` |
| **Linux** | `~/.local/share/ORF` | `~/.config/ORF` | `~/.cache/ORF` |

On Linux, ORF follows `XDG_DATA_HOME`, `XDG_CONFIG_HOME` and `XDG_CACHE_HOME` when they are set.

## Thumbnail cache

Thumbnails are saved in the cache directory. If thumbnails look wrong or the cache is large, close ORF and delete the folder. ORF creates the thumbnails again when you open a folder.

## Reset ORF

1. Close ORF.
2. Delete the config directory to reset preferences, themes and layouts.
3. Delete the data directory to remove the index and installed plugins.

!!! note
    The assets folder is not in the data directory, so resetting ORF does not delete it. It is `Pictures/ORF` unless you chose another folder, and it holds the images that ORF saved from your browser.

Your own files are never stored in these directories, except for the [ORF Folder](../plugins/orf-folder.md) if you use it.

## Related topics

- [Concepts](../introduction/concepts.md)
- [Workspaces](../introduction/workspaces.md)
- [FAQ](../../faq/index.md)
