#!/usr/bin/env python3
"""Build and check the localized ORF documentation.

English (docs/en) is the source of truth. Every other language is built from a
staged copy of English with the translated pages (docs/<lang>) laid on top.
Pages without a translation fall back to English with a banner.

Commands:
  build           build the English site and every enabled language into site/
  status          list translated, outdated, missing and orphan pages per language
  stamp <page>... record the current English version in a translated page

See docs/en/development/translating.md.
"""
import hashlib
import json
import re
import shutil
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DOCS = ROOT / "docs"
EN = DOCS / "en"
I18N = ROOT / "i18n"
BUILD = ROOT / "build"
SITE = ROOT / "site"
CONFIG = ROOT / "zensical.toml"
LANGS = ["ja", "ko", "zh", "zh-Hant"]
IGNORED = {".gitkeep", ".DS_Store"}

DEFAULT_BANNERS = {
    "banner_untranslated": "This page is not translated yet. It shows the English version.",
    "banner_outdated": "This translation may be out of date. The English page has changed since it was translated.",
}

FRONT_MATTER = re.compile(r"\A---\r?\n(.*?)\r?\n---\r?\n", re.S)
SOURCE_LINE = re.compile(r"^source:\s*(\S+)\s*$", re.M)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def q(s):
    """TOML basic string."""
    return json.dumps(s, ensure_ascii=False)


def pages(base):
    """Markdown pages under base, as sorted paths relative to base."""
    return sorted(
        p.relative_to(base) for p in base.rglob("*.md") if p.name not in IGNORED
    )


def en_hash(rel):
    """Version of an English page: the git blob hash of its content."""
    path = EN / rel
    try:
        out = subprocess.run(
            ["git", "hash-object", str(path)],
            cwd=ROOT, capture_output=True, text=True, check=True,
        )
        return out.stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        data = path.read_bytes()
        return hashlib.sha1(b"blob %d\0" % len(data) + data).hexdigest()


def source_of(path):
    m = FRONT_MATTER.match(path.read_text(encoding="utf-8"))
    if not m:
        return None
    s = SOURCE_LINE.search(m.group(1))
    return s.group(1) if s else None


def load_lang(lang):
    return tomllib.loads((I18N / f"{lang}.toml").read_text(encoding="utf-8"))


def enabled_langs():
    return [l for l in LANGS if load_lang(l).get("enabled", False)]


def zensical_cmd():
    exe = "zensical.exe" if sys.platform == "win32" else "zensical"
    local = Path(sys.executable).parent / exe
    if local.exists():
        return [str(local)]
    found = shutil.which("zensical")
    if found:
        return [found]
    sys.exit("zensical not found. Run: pip install -r requirements.txt")


def run_zensical(config):
    cmd = zensical_cmd() + ["build", "--clean", "-f", str(config)]
    print("+", " ".join(cmd[1:]))
    subprocess.run(cmd, cwd=ROOT, check=True)


# ---------------------------------------------------------------------------
# Config generation
# ---------------------------------------------------------------------------
def nav_to_toml(items, indent=0):
    pad = "  " * (indent + 1)
    rows = []
    for item in items:
        for title, value in item.items():
            if isinstance(value, list):
                rows.append(f"{pad}{{ {q(title)} = {nav_to_toml(value, indent + 1)} }}")
            else:
                rows.append(f"{pad}{{ {q(title)} = {q(value)} }}")
    return "[\n" + ",\n".join(rows) + "\n" + "  " * indent + "]"


def translate_nav(items, titles):
    out = []
    for item in items:
        new = {}
        for title, value in item.items():
            t = titles.get(title) or title
            new[t] = translate_nav(value, titles) if isinstance(value, list) else value
        out.append(new)
    return out


def alternates(langs):
    entries = [("English", "en")] + [(load_lang(l)["name"], l) for l in langs]
    return "".join(
        f'\n[[project.extra.alternate]]\nname = {q(name)}\nlink = "/{lang}/"\nlang = {q(lang)}\n'
        for name, lang in entries
    )


def english_config(langs):
    """Path of the config to build English with."""
    if not langs:
        return CONFIG
    out = ROOT / ".zensical.en.toml"
    out.write_text(CONFIG.read_text(encoding="utf-8") + alternates(langs), encoding="utf-8")
    return out


def language_config(lang, langs):
    cfg = load_lang(lang)
    base = tomllib.loads(CONFIG.read_text(encoding="utf-8"))["project"]
    text = CONFIG.read_text(encoding="utf-8")

    def sub(pattern, repl):
        nonlocal text
        text, n = re.subn(pattern, lambda _: repl, text, count=1, flags=re.M)
        if n != 1:
            sys.exit(f"zensical.toml: could not rewrite {pattern!r}")

    sub(r'^docs_dir = .*$', f'docs_dir = {q(f"build/{lang}")}')
    sub(r'^site_dir = .*$', f'site_dir = {q(f"site/{lang}")}')
    sub(r'^site_url = .*$', f'site_url = {q(base["site_url"].removesuffix("en/") + lang + "/")}')
    sub(r'^language = .*$', f'language = {q(cfg["theme_language"])}')
    if cfg.get("site_name"):
        sub(r'^site_name = .*$', f'site_name = {q(cfg["site_name"])}')
    if cfg.get("site_description"):
        sub(r'^site_description = .*$', f'site_description = {q(cfg["site_description"])}')
    nav = translate_nav(base["nav"], cfg.get("nav", {}))
    sub(r'^nav = \[(?s:.*?)^\]$', "nav = " + nav_to_toml(nav))
    text += alternates(langs)

    out = ROOT / f".zensical.{lang}.toml"
    out.write_text(text, encoding="utf-8")
    return out


# ---------------------------------------------------------------------------
# Staging
# ---------------------------------------------------------------------------
def add_banner(path, kind, text):
    """Insert a warning admonition below the front matter of a page."""
    body = path.read_text(encoding="utf-8")
    banner = f'!!! warning "{kind}"\n\n    {text}\n\n'
    m = FRONT_MATTER.match(body)
    pos = m.end() if m else 0
    path.write_text(body[:pos] + banner + body[pos:], encoding="utf-8")


def stage(lang):
    cfg = load_lang(lang)
    texts = {k: cfg.get(k) or v for k, v in DEFAULT_BANNERS.items()}
    dest = BUILD / lang
    shutil.copytree(EN, dest)

    translated = {}
    src = DOCS / lang
    for p in sorted(src.rglob("*")):
        if p.is_file() and p.name not in IGNORED:
            rel = p.relative_to(src)
            (dest / rel).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, dest / rel)
            if p.suffix == ".md":
                translated[rel] = p

    for rel in pages(EN):
        if rel not in translated:
            add_banner(dest / rel, "Not translated", texts["banner_untranslated"])
        elif source_of(translated[rel]) != en_hash(rel):
            add_banner(dest / rel, "Out of date", texts["banner_outdated"])
    return dest


# ---------------------------------------------------------------------------
# Commands
# ---------------------------------------------------------------------------
def redirect_page(langs):
    mapping = {"en": "en"}
    for lang in langs:
        mapping[lang.lower()] = lang
        if lang == "zh-Hant":
            mapping.update({"zh-tw": lang, "zh-hk": lang, "zh-mo": lang})
    items = ",\n".join(f"      {q(k)}: {q(v)}" for k, v in mapping.items())
    links = "\n".join(
        f'    <li><a href="{l}/">{n}</a></li>'
        for n, l in [("English", "en")] + [(load_lang(l)["name"], l) for l in langs]
    )
    return f"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ORF Documentation</title>
  <meta http-equiv="refresh" content="0; url=en/">
  <script>
    // Send the browser to the best language. Only enabled languages are listed.
    var languageMap = {{
{items}
    }};
    var userLang = (navigator.language || navigator.userLanguage || "en").toLowerCase();
    var target = languageMap[userLang] || languageMap[userLang.substring(0, 2)] || "en";
    window.location.replace(target + "/");
  </script>
</head>
<body>
  <p>Select a language:</p>
  <ul>
{links}
  </ul>
</body>
</html>
"""


def cmd_build():
    for d in (SITE, BUILD):
        shutil.rmtree(d, ignore_errors=True)
    langs = enabled_langs()
    run_zensical(english_config(langs))
    for lang in langs:
        stage(lang)
        run_zensical(language_config(lang, langs))
    (SITE / "index.html").write_text(redirect_page(langs), encoding="utf-8")
    shutil.copy2(SITE / "en" / "404.html", SITE / "404.html")
    print(f"Built English and {len(langs)} language(s): {', '.join(langs) or 'none enabled'}")


def glossary_gaps(lang):
    terms = tomllib.loads((I18N / "glossary.toml").read_text(encoding="utf-8")).get("terms", {})
    return [t for t, v in terms.items() if not v.get(lang)]


def cmd_status():
    failed = False
    english = pages(EN)
    for lang in LANGS:
        enabled = load_lang(lang).get("enabled", False)
        src = DOCS / lang
        have = pages(src) if src.exists() else []
        current, outdated, unstamped = [], [], []
        for rel in have:
            if rel not in english:
                continue
            s = source_of(src / rel)
            if s is None:
                unstamped.append(rel)
            elif s != en_hash(rel):
                outdated.append(rel)
            else:
                current.append(rel)
        missing = [r for r in english if r not in have]
        orphans = [r for r in have if r not in english]
        gaps = glossary_gaps(lang)

        print(f"\n{lang} ({'enabled' if enabled else 'disabled'})")
        print(f"  translated: {len(current)}  outdated: {len(outdated)}  unstamped: {len(unstamped)}"
              f"  missing: {len(missing)}  orphan: {len(orphans)}  glossary gaps: {len(gaps)}")
        for label, items in (("outdated", outdated), ("unstamped", unstamped), ("orphan", orphans)):
            for rel in items:
                print(f"    {label}: {rel}")
        if gaps:
            print(f"    glossary gaps: {', '.join(gaps)}")
        if enabled and (outdated or unstamped or orphans or gaps):
            failed = True
    print("\nProblems in enabled languages." if failed else "\nNo problems in enabled languages.")
    return 1 if failed else 0


def cmd_stamp(args):
    if not args:
        sys.exit("usage: build_docs.py stamp <page>...")
    for arg in args:
        path = Path(arg).resolve()
        try:
            lang, *rest = path.relative_to(DOCS).parts
        except ValueError:
            sys.exit(f"{arg}: not under docs/")
        rel = Path(*rest)
        if lang not in LANGS or not path.is_file() or path.suffix != ".md":
            sys.exit(f"{arg}: not a translated page (docs/<lang>/<page>.md)")
        if rel not in pages(EN):
            sys.exit(f"{arg}: no English page docs/en/{rel}")
        digest = en_hash(rel)
        body = path.read_text(encoding="utf-8")
        m = FRONT_MATTER.match(body)
        if m and SOURCE_LINE.search(m.group(1)):
            fm = SOURCE_LINE.sub(f"source: {digest}", m.group(1))
            body = f"---\n{fm}\n---\n" + body[m.end():]
        elif m:
            body = f"---\n{m.group(1)}\nsource: {digest}\n---\n" + body[m.end():]
        else:
            body = f"---\nsource: {digest}\n---\n\n" + body
        path.write_text(body, encoding="utf-8")
        print(f"stamped {path.relative_to(ROOT)} -> {digest[:10]}")


def main(argv):
    if not argv or argv[0] not in ("build", "status", "stamp"):
        print(__doc__)
        return 2
    if argv[0] == "build":
        cmd_build()
    elif argv[0] == "status":
        return cmd_status()
    else:
        cmd_stamp(argv[1:])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
