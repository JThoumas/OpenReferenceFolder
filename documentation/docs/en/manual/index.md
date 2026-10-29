# Getting Started

This page walks you through your first session in OpenReferenceFolder (ORF).

!!! warning "In development"
    ORF is under active development and has no stable release yet. This manual describes how ORF is designed to work, and some features may be missing or different in the build you run. The [Roadmap](../development/roadmap.md) lists what is planned. Installers are available with the first release. Until then, you can [build ORF from source](../development/building.md).

## 1. Install ORF

<div class="os-grid">
  <div class="os-card">
    <div class="os-header">Windows</div>
    <div class="os-content">
      <img src="../assets/platform/windows-logo.svg" alt="Windows" class="os-logo">
    </div>
    <div class="os-actions">
      <a href="installation/windows" class="os-btn primary">Installation Guide</a>
    </div>
  </div>

  <div class="os-card">
    <div class="os-header">macOS</div>
    <div class="os-content">
      <img src="../assets/platform/macos-logo.svg" alt="macOS" class="os-logo">
    </div>
    <div class="os-actions">
      <a href="installation/macos" class="os-btn primary">Installation Guide</a>
    </div>
  </div>

  <div class="os-card">
    <div class="os-header">Linux</div>
    <div class="os-content">
      <img src="../assets/platform/linux-logo.svg" alt="Linux" class="os-logo">
    </div>
    <div class="os-actions">
      <a href="installation/linux" class="os-btn primary">Installation Guide</a>
    </div>
  </div>
</div>

## 2. First steps

### Add a folder

1. Start ORF. The first time, ORF asks you to choose an assets folder. If you skip it, ORF uses `Pictures/ORF`.
2. In the navigation tree of the file browser, click **+** and choose a folder or files in the system dialog. See [Homepage](introduction/homepage.md).
3. ORF adds them as top-level entries of the tree. Your files stay where they are.

Read [Concepts](introduction/concepts.md) for how virtual folders, real folders and files relate.

### Browse

1. Open a folder from the navigation tree.
2. Switch between grid and list view, and sort the contents. See [File Browser](introduction/file-browser.md).
3. Turn on the [Reference Filter](introduction/filtering.md) to hide files that are not images or project files.

### Reference with PureView

1. Hover over an image. PureView previews it.
2. Click an image to select it. When you move the pointer away, PureView goes back to the image you selected.
3. Double-click an image to open it in PureView.

See [PureView](plugins/pureview.md) for zoom, rotate, flip and the floating viewer.

### Arrange your panels

1. Drag a panel's tab to the edge of the window. A docking guide appears. Drop the panel on one of its targets.
2. Drop a panel onto another panel to stack them as tabs.
3. Drag a tab out of the window to make a floating window.

ORF saves the arrangement when you close it. See [Docking Panels](introduction/docking-panels.md) and [Workspaces](introduction/workspaces.md).

## Where to go next

- [Concepts](introduction/concepts.md): the ideas behind ORF.
- [File Operations](introduction/file-operations.md) and [Drag and Drop](introduction/drag-and-drop.md): working with files.
- [Preferences](preferences/index.md): themes, default applications, exclusions and [Compact Mode](introduction/compact-mode.md).
- [Pen Tablets](pen-tablets.md): using ORF with a pen.
- [FAQ](../faq/index.md): common questions.
