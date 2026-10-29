# Pen Tablets

ORF is designed to work with a pen tablet or a pen display, without a mouse or keyboard.

## Overview

ORF treats the pen as a **pointer**. A pen tap is a click, and a drag is a drag. ORF does not read pen pressure or tilt. It does not need a special driver beyond the one your tablet already uses.

## Pen-friendly interface

- **Large targets.** Buttons, tabs and resize grips are sized to be hit with a pen tip.
- **Long-press menu.** Press and hold an item to open its context menu, in place of a right-click.
- **Drag to scroll.** Drag on lists and grids to scroll them.

## Set up your tablet

Your tablet driver controls how the pen moves the pointer. For ORF, set the pen to move the system pointer.

=== "OpenTabletDriver"

    [OpenTabletDriver](https://opentabletdriver.net/) is an open-source tablet driver for Windows, macOS and Linux. Use an output mode that moves the pointer, such as Absolute mode, and map the tablet area to the monitor that shows ORF.

=== "Windows Ink"

    On Windows, many tablet drivers offer **Windows Ink**. ORF works with it on or off, because ORF only uses the pointer position and the pen buttons. Windows Ink does not change how ORF reads input.

=== "Other drivers"

    Drivers from the tablet maker work the same way, as long as they move the system pointer.

## Limits

- Pen pressure and tilt are not used.
- ORF is tested with desktop tablets and pen displays. Phones and tablets with a touch screen only are not supported.
- Pen behavior on Linux is not tested on all desktop environments.

## Related topics

- [Docking Panels](introduction/docking-panels.md)
- [File Browser](introduction/file-browser.md)
- [FAQ](../faq/index.md)
