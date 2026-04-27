#pragma once
#include "theme.h"
#include <string>
#include <vector>
#include <functional>

namespace orf {

class ThemeManager {
public:
    // Global singleton access — one active theme per application instance
    static ThemeManager& get();

    // Load a theme from a JSON file.
    // Falls back to the built-in dark theme if the file is missing or malformed.
    bool loadFromFile(const std::string& path);

    // Save the current theme to a JSON file (for the theme editor)
    bool saveToFile(const std::string& path) const;

    // Reset to the built-in default dark theme
    void resetToDefault();

    // The currently active theme — all widgets read from this
    const Theme& current() const { return m_current; }
          Theme& current()       { return m_current; }

    // Register a callback that fires when the theme changes —
    // widgets use this to mark themselves dirty on theme swap
    using ThemeChangedCallback = std::function<void()>;
    void onThemeChanged(ThemeChangedCallback cb);
    void notifyChanged();

    // Built-in themes bundled with ORF
    static Theme darkTheme();
    static Theme lightTheme();

private:
    ThemeManager() { m_current = darkTheme(); }

    Theme                             m_current;
    std::vector<ThemeChangedCallback> m_callbacks;
};

// Convenience shorthand used throughout the codebase
// Usage: orf::theme().browserBackground
inline const Theme& theme() { return ThemeManager::get().current(); }

} // namespace orf
