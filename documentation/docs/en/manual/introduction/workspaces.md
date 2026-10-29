# Workspaces

A workspace is a saved arrangement of your ORF panels and windows.

## Overview

A workspace stores:

- which panels are open and where they are docked,
- tab groups and the size of each region,
- floating windows, with their position, size and monitor,
- the plugin windows that were open.

ORF does not store your files in a workspace. See [Docking Panels](docking-panels.md) for how to arrange panels.

## Automatic saving

ORF saves the current layout when you close it and restores it the next time you start. You do not have to do anything.

## Named workspaces

Use named workspaces to switch between arrangements, for example one for browsing and one for drawing with a floating reference window.

1. Arrange your panels the way you want.
2. Open **Window > Workspace** in the top bar.
3. Choose **Save** and enter a name.

To switch, choose **Window > Workspace > Load** and select a workspace. Choose **Manage** to rename or delete workspaces.

## If a layout file is bad

ORF checks a layout file when it loads it. If the file cannot be read:

1. ORF keeps a copy of the bad file next to the original, so you can inspect it.
2. ORF loads the last layout that worked.
3. If there is none, ORF starts with the default layout.

Your files and your index are not affected.

## When your monitors change

If you unplug a monitor or change the resolution, windows that were on it could end up off screen. ORF moves them onto a monitor that is connected when it restores the layout.

## Related topics

- [Docking Panels](docking-panels.md)
- [Homepage](homepage.md)
- [Data Locations](../installation/data-locations.md)
