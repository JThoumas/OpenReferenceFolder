# Docking Panels

Every part of the ORF interface is a panel that you can move, stack or detach.

## Overview

The file browser, preferences and plugins such as PureView are panels. ORF arranges panels in a tree of regions. Each region holds one panel or a group of tabs.

## Tabs

Panels in the same region are shown as tabs.

- Click a tab to show its panel.
- Drag a tab left or right to change its position.
- Close a tab with its **×** button.

## Dock a panel

1. Press and hold a panel's tab, then drag it over the window.
2. A **docking guide** appears. It shows the places where the panel can go.
3. Move over a target and release.

| Target | Result |
| --- | --- |
| Left, right, top or bottom edge | The panel snaps to that side of the window or region. |
| Center of another panel | The panel is stacked with the other panel as tabs. |

The grips between regions are large enough for a pen or a finger. Drag a grip to resize the regions on both sides.

## Floating windows

Drag a tab outside the main window to make a floating window. It is an independent window, so you can place it on any monitor.

To bring it back, drag its tab onto the main window and use the docking guide.

### Borderless viewers

Floating plugin windows such as image viewers can hide their title bar and border. They look like an image placed on your screen. Right-click or long-press the window to open its menu and show the title bar again.

### Link border

A floating PureView is linked to a file browser. When you hover over or select a valid file in that file browser, the border of the linked floating window lights up. This shows which window shows the file.

### Always on top

Turn on **Always on top** for a floating window to keep it above other applications, for example above your drawing software.

!!! note
    Always-on-top behavior depends on your operating system and window manager. See the [platform FAQ](../../faq/platforms.md).

## Multiple monitors

Floating windows can be on any monitor. ORF remembers which monitor each window was on and restores it. See [Workspaces](workspaces.md).

## Related topics

- [Workspaces](workspaces.md)
- [Compact Mode](compact-mode.md)
- [PureView](../plugins/pureview.md)
