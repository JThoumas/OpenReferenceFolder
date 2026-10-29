# Development Guide

Thank you for helping with ORF. This page explains how to set up your environment and make changes to ORF.

## Overview

Development happens on [GitHub](https://github.com/JThoumas/OpenReferenceFolder). There are three ways to help:

- **Documentation:** fix or improve a page. You only need a text editor and the steps in [Documentation](#documentation).
- **Code changes:** follow [Set up your environment](#set-up-your-environment) and [Make a change](#make-a-change).
- **Full setup:** set up an IDE. See [Visual Studio (Windows)](#visual-studio-windows) or [VS Code](#vs-code).

!!! tip

    Read [Architecture](architecture.md) first. It shows how the framework, the app code and the components interact with each other.

## Set up your environment

1. Fork [`JThoumas/OpenReferenceFolder`](https://github.com/JThoumas/OpenReferenceFolder) on GitHub.

2. Clone your fork:

    ```bash
    git clone https://github.com/<your-name>/OpenReferenceFolder.git
    cd OpenReferenceFolder
    ```

    ORF has no Git submodules. CMake downloads the dependencies with `FetchContent` when you configure.

3. Install the requirements. See [Requirements](building.md#requirements).

4. Configure and build ORF. See [Build](building.md#build).

5. Run the program from the `build` folder. On Windows with a multi-config generator, it is in `build\Release`. ORF does not store data in the working directory. See [Data Locations](../manual/installation/data-locations.md).

## Make a change

1. Create a branch from `main`:

    ```bash
    git switch -c feat/list-view
    ```

2. Make your change, build it and run it.

3. Commit with a [Conventional Commits](https://www.conventionalcommits.org/) message:

    ```bash
    git commit -m "feat(browser): add list view"
    ```

4. Push the branch to your fork:

    ```bash
    git push -u origin feat/list-view
    ```

5. Open a pull request against `main` on GitHub. See [Pull requests](#pull-requests).

### Sync with upstream

Add the original repository as `upstream` once, then bring your branch up to date whenever `main` moves:

```bash
git remote add upstream https://github.com/JThoumas/OpenReferenceFolder.git
git fetch upstream
git rebase upstream/main
```

Rebasing is the recommended way. If you prefer, run `git merge upstream/main` instead of the last command.

!!! tip

    The Git panels in Visual Studio and VS Code can do all of these steps for you. You do not need the command line.

## Visual Studio (Windows)

Install [Visual Studio](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload. It includes the MSVC compiler and CMake support.

=== "Open Folder"

    Visual Studio opens ORF as a CMake project.

    1. Choose **File > Open > Folder** and select the `OpenReferenceFolder` folder. Visual Studio configures CMake and downloads the dependencies.
    2. Choose `OpenReferenceFolder.exe` in the **Select Startup Item** list.
    3. Press `F5` to build and debug.

=== "Solution"

    Generate a Visual Studio solution and open it.

    1. Generate the solution:

        ```bash
        cmake -B build -G "Visual Studio 17 2022" -A x64
        ```

    2. Open `build/OpenReferenceFolder.sln`.
    3. In **Solution Explorer**, right-click `OpenReferenceFolder` and choose **Set as Startup Project**.
    4. Press `F5` to build and debug.

## VS Code

Install these extensions:

| Extension | Use |
| --- | --- |
| CMake Tools | Configure, build and run with CMake |
| clangd | Code completion and navigation |
| C/C++ | Debugger on Windows |
| CodeLLDB | Debugger on macOS and Linux |

!!! tip

    If you use clangd, set `C_Cpp.intelliSenseEngine` to `disabled` so the C/C++ extension does not compete with it.

1. Open the `OpenReferenceFolder` folder in VS Code.
2. Select a kit when CMake Tools asks for one.
3. Select the build variant, for example **Debug**.
4. For clangd, add `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` to `cmake.configureArgs` in your settings. This works only with the Ninja and Makefile generators.

Useful shortcuts:

- `F7` builds ORF.
- `Ctrl+F5` builds and starts the debugger.
- `Shift+F5` runs ORF without the debugger.

## Code style

ORF uses `clang-format` with the **WebKit** style. The `.clang-format` file is at the root of the repository. Format your changes before you commit:

```bash
clang-format -i path/to/file.cpp
```

CI checks formatting of `src/` and `tests/`. Third-party code in `extern/` is not checked.

## Tests

Tests use Catch2 and CTest. They cover the code in `src/core`, which does not need OpenGL.

- Add a test for new logic in `src/core`.
- Build with `-DORF_BUILD_TESTS=ON` and run `ctest`. See [Building ORF](building.md#tests).

## Issues

Report bugs and suggest features in [GitHub Issues](https://github.com/JThoumas/OpenReferenceFolder/issues). Search the existing issues first so the same report is not filed twice.

- **Bug reports:** include your operating system, the ORF version or commit, the steps to reproduce the bug, what you expected and what happened. Add screenshots if they help.
- **Feature requests:** describe what you want to do and why. A use case helps more than a proposed solution.
- **Large changes:** open an issue before you start, so the approach can be agreed first.

## Pull requests

- Use [Conventional Commits](https://www.conventionalcommits.org/) in commit messages and PR titles, for example `feat(browser): add list view`.
- CI builds and tests on Windows, macOS and Linux. The **`ci-ok`** check must pass before a pull request can be merged. It also passes when a change touches only documentation.

## AI disclaimer and policy

OpenReferenceFolder uses generative AI for its development.

When contributing to OpenReferenceFolder, AI usage is not discouraged. We welcome our developers and users to develop plugins and features they want locally, and even better, share them by contributing to the ORF GitHub repository.

However, full agentic development is overkill and is discouraged. Do not make contributions that change all the components of ORF in one go.

If your AI tool supports skills, you may use the skills stored in the repository `.agent/skills` to contribute correctly to ORF.

## Documentation

The documentation is built with [Zensical](https://zensical.org). Pages are Markdown files in `docs/en`.

1. Install the tools:

    ```bash
    python3 -m venv .venv
    .venv/bin/pip install -r requirements.txt
    ```

2. Preview with live reload:

    ```bash
    .venv/bin/zensical serve
    ```

    This previews the English site only.

3. Build the whole site, with every enabled language, to check for errors:

    ```bash
    .venv/bin/python scripts/build_docs.py build
    ```

To translate the documentation, see [Translating](translating.md).

## License

By contributing, you agree that your contribution is licensed under the AGPL-3.0.

## Related topics

- [Building ORF](building.md)
- [Architecture](architecture.md)
- [Plugin Development](plugin-development.md)
- [Translating](translating.md)
- [Roadmap](roadmap.md)
