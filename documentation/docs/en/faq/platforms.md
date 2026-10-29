# Platform Questions

Answers that depend on your operating system. Pick your system with the tabs.

=== "Windows"

    ### Windows SmartScreen blocks ORF

    ORF releases are not code-signed, so SmartScreen may show "Windows protected your PC". Select **More info**, check that the file came from the [releases page](https://github.com/JThoumas/OpenReferenceFolder/releases), then select **Run anyway**.

    ### ORF cannot save its settings

    This applies to portable mode (the zip). You probably extracted ORF into a folder that needs administrator permission, such as `C:\` or `C:\Program Files\`. Move the ORF folder to a place you can write to, for example `D:\Apps\ORF` or your user folder. An installed ORF stores its data in `%APPDATA%\ORF`. See [Data Locations](../manual/installation/data-locations.md).

    ### ORF does not start, or the window is black

    ORF needs OpenGL 3.3. Update your graphics driver. Remote desktop sessions and virtual machines often provide only an older OpenGL version.

    ### Windows 7 and 8.1

    They are on the support table, but older Windows versions are not verified yet. Report problems on GitHub.

=== "macOS"

    ### macOS says ORF cannot be opened

    ORF is not signed or notarized, because the project does not have an Apple Developer membership. Follow [Open an unsigned app](../manual/installation/macos.md#open-an-unsigned-app).

    ### macOS says "ORF is damaged and can't be opened"

    macOS adds a quarantine flag to downloaded apps. Remove it in Terminal, after you check that the download came from the releases page:

    ```bash
    xattr -dr com.apple.quarantine /Applications/OpenReferenceFolder.app
    ```

    ### ORF cannot see my Documents, Desktop or external drive

    Allow access in **System Settings > Privacy & Security > Files and Folders**. For a whole external drive, you may also need **Full Disk Access**.

    ### Older macOS versions

    macOS 10.15 and 11 are on the support table. They are not verified yet.

=== "Linux"

    ### Does ORF run on Wayland?

    ORF can run on Wayland and on X11. Wayland is not tested on every desktop environment. Some window features depend on the compositor, in particular always-on-top and window positions for floating windows. If a floating window cannot be placed or kept on top, try an X11 session and report the compositor you use.

    ### ORF fails with an OpenGL error

    ORF needs OpenGL 3.3 core. Check your version:

    ```bash
    glxinfo | grep "OpenGL core profile version"
    ```

    If it shows a lower version, update your graphics drivers or Mesa. Software rendering in a virtual machine may not offer 3.3 core.

    ### The AppImage does not start

    Install FUSE 2, or run it with `--appimage-extract-and-run`. See [Linux installation](../manual/installation/linux.md).

    ### Native dialogs look wrong

    ORF uses GTK or the desktop portal for file dialogs. Install the portal for your desktop (`xdg-desktop-portal` and its backend) if dialogs do not open.

## Related topics

- [FAQ](index.md)
- [Installation Guides](../manual/installation/windows.md)
