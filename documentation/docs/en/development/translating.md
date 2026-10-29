# Translating

This page explains how the ORF documentation is translated and how to help.

## Overview

English is the source of truth. The other languages are ja (日本語), ko (한국어), zh (简体中文) and zh-Hant (繁體中文).

No translated site is published yet. Until the documentation stabilises, everyone gets English. A language goes live when it is enabled (see [Enable a language](#enable-a-language)).

Translations are drafted by Claude and reviewed by native speakers through pull requests.

## How it works

- A translated page has the same path under `docs/<lang>/` as the English page under `docs/en/`. For example, `docs/en/manual/index.md` is translated as `docs/ja/manual/index.md`.
- A page that is not translated falls back to the English page, with a banner at the top. The navigation is always complete.
- Each translated page records the English version it came from. When the English page changes, the translation is marked outdated and gets a different banner.
- Page titles in the navigation are translated in `i18n/<lang>.toml`. A missing title falls back to English.

## Translate a page

1. Copy the English page to the same path under `docs/<lang>/` and translate it.

2. Use the [glossary](#glossary). Keep Markdown, links, code blocks and file names unchanged.

3. Record the English version the page came from:

    ```bash
    .venv/bin/python scripts/build_docs.py stamp docs/ja/manual/index.md
    ```

    This writes a `source:` line into the front matter of the page. It holds the version hash of the English page.

4. Check the result:

    ```bash
    .venv/bin/python scripts/build_docs.py status
    ```

5. Open a pull request. A native speaker reviews it.

!!! note

    Run `stamp` only after you have checked the translation against the current English page. `stamp` does not compare the content.

## Glossary

`i18n/glossary.toml` lists the ORF terms. It has two groups:

- **Never translated:** ORF, OpenReferenceFolder, PureView, ZenRef and ORF Folder.
- **Translated:** terms such as Compact mode, virtual folder, workspace and Preferences. Each language has one translation per term.

Use the app's own strings. Never invent a term. If a term is missing from the glossary, add it to the glossary first. The strings in the app must use the same terms as the documentation.

## Enable a language

Set `enabled = true` in `i18n/<lang>.toml` when the language is ready. Before you do:

- Fill in the glossary for the language.
- Translate the site name, the site description, the navigation titles and the two banner texts in `i18n/<lang>.toml`.
- Translate the pages and run `stamp` on each one.
- Run `status`. It must report no problems for the language.

An enabled language is added to the language switcher and to the language redirect at the root of the site.

## Related topics

- [Development Guide](development-guide.md)
- [Roadmap](roadmap.md)
