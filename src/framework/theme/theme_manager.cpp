#include "theme_manager.h"
#include "framework/core/log.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace orf {

// Helper macros for JSON load/save of Color fields
#define LOAD_COLOR(j, t, field) \
    if ((j).contains(#field)) { \
        auto& arr = (j)[#field]; \
        (t).field = {arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>(), arr[3].get<float>()}; \
    }

#define SAVE_COLOR(j, t, field) \
    (j)[#field] = {(t).field.r, (t).field.g, (t).field.b, (t).field.a}

ThemeManager& ThemeManager::get() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::notifyChanged() {
    for (auto& cb : m_callbacks) cb();
}

void ThemeManager::onThemeChanged(ThemeChangedCallback cb) {
    m_callbacks.push_back(std::move(cb));
}

void ThemeManager::resetToDefault() {
    m_current = darkTheme();
    notifyChanged();
}

bool ThemeManager::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        LOG_WARN("ThemeManager: could not open " << path << " — using default");
        return false;
    }

    try {
        json j = json::parse(file);
        Theme t;

        if (j.contains("name"))   t.name   = j["name"];
        if (j.contains("author")) t.author = j["author"];

        LOAD_COLOR(j, t, windowBackground);
        LOAD_COLOR(j, t, panelBackground);
        LOAD_COLOR(j, t, panelBorder);
        LOAD_COLOR(j, t, titleBarBackground);
        LOAD_COLOR(j, t, titleBarText);
        LOAD_COLOR(j, t, tabBarBackground);
        LOAD_COLOR(j, t, tabActive);
        LOAD_COLOR(j, t, tabInactive);
        LOAD_COLOR(j, t, tabActiveText);
        LOAD_COLOR(j, t, tabInactiveText);
        LOAD_COLOR(j, t, buttonNormal);
        LOAD_COLOR(j, t, buttonHover);
        LOAD_COLOR(j, t, buttonPressed);
        LOAD_COLOR(j, t, buttonText);
        LOAD_COLOR(j, t, divider);
        LOAD_COLOR(j, t, dividerHovered);
        LOAD_COLOR(j, t, scrollbarTrack);
        LOAD_COLOR(j, t, scrollbarThumb);
        LOAD_COLOR(j, t, browserBackground);
        LOAD_COLOR(j, t, browserRowOdd);
        LOAD_COLOR(j, t, browserSelected);
        LOAD_COLOR(j, t, browserSelectedText);
        LOAD_COLOR(j, t, browserText);
        LOAD_COLOR(j, t, folderIcon);
        LOAD_COLOR(j, t, textPrimary);
        LOAD_COLOR(j, t, textSecondary);
        LOAD_COLOR(j, t, textDisabled);
        LOAD_COLOR(j, t, contextMenuBackground);
        LOAD_COLOR(j, t, contextMenuHover);
        LOAD_COLOR(j, t, contextMenuText);
        LOAD_COLOR(j, t, contextMenuSeparator);
        LOAD_COLOR(j, t, accent);
        LOAD_COLOR(j, t, accentHover);

        m_current = t;
        notifyChanged();
        LOG_INFO("ThemeManager: loaded theme '" << t.name << "' from " << path);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("ThemeManager: parse error in " << path << " — " << e.what());
        return false;
    }
}

bool ThemeManager::saveToFile(const std::string& path) const {
    json j;
    const Theme& t = m_current;

    j["name"]   = t.name;
    j["author"] = t.author;

    SAVE_COLOR(j, t, windowBackground);
    SAVE_COLOR(j, t, panelBackground);
    SAVE_COLOR(j, t, panelBorder);
    SAVE_COLOR(j, t, titleBarBackground);
    SAVE_COLOR(j, t, titleBarText);
    SAVE_COLOR(j, t, tabBarBackground);
    SAVE_COLOR(j, t, tabActive);
    SAVE_COLOR(j, t, tabInactive);
    SAVE_COLOR(j, t, tabActiveText);
    SAVE_COLOR(j, t, tabInactiveText);
    SAVE_COLOR(j, t, buttonNormal);
    SAVE_COLOR(j, t, buttonHover);
    SAVE_COLOR(j, t, buttonPressed);
    SAVE_COLOR(j, t, buttonText);
    SAVE_COLOR(j, t, divider);
    SAVE_COLOR(j, t, dividerHovered);
    SAVE_COLOR(j, t, scrollbarTrack);
    SAVE_COLOR(j, t, scrollbarThumb);
    SAVE_COLOR(j, t, browserBackground);
    SAVE_COLOR(j, t, browserRowOdd);
    SAVE_COLOR(j, t, browserSelected);
    SAVE_COLOR(j, t, browserSelectedText);
    SAVE_COLOR(j, t, browserText);
    SAVE_COLOR(j, t, folderIcon);
    SAVE_COLOR(j, t, textPrimary);
    SAVE_COLOR(j, t, textSecondary);
    SAVE_COLOR(j, t, textDisabled);
    SAVE_COLOR(j, t, contextMenuBackground);
    SAVE_COLOR(j, t, contextMenuHover);
    SAVE_COLOR(j, t, contextMenuText);
    SAVE_COLOR(j, t, contextMenuSeparator);
    SAVE_COLOR(j, t, accent);
    SAVE_COLOR(j, t, accentHover);

    std::ofstream file(path);
    if (!file.is_open()) {
        LOG_ERROR("ThemeManager: could not write to " << path);
        return false;
    }
    file << j.dump(2);
    LOG_INFO("ThemeManager: saved theme '" << t.name << "' to " << path);
    return true;
}

Theme ThemeManager::darkTheme() {
    return Theme{}; // all defaults are already dark
}

Theme ThemeManager::lightTheme() {
    Theme t;
    t.name                  = "ORF Light";
    t.windowBackground      = {0.92f, 0.92f, 0.93f, 1.0f};
    t.panelBackground       = {0.96f, 0.96f, 0.97f, 1.0f};
    t.panelBorder           = {0.78f, 0.78f, 0.80f, 1.0f};
    t.titleBarBackground    = {0.86f, 0.86f, 0.88f, 1.0f};
    t.titleBarText          = {0.15f, 0.15f, 0.18f, 1.0f};
    t.tabBarBackground      = {0.82f, 0.82f, 0.84f, 1.0f};
    t.tabActive             = {0.96f, 0.96f, 0.97f, 1.0f};
    t.tabInactive           = {0.86f, 0.86f, 0.88f, 1.0f};
    t.tabActiveText         = {0.10f, 0.10f, 0.12f, 1.0f};
    t.tabInactiveText       = {0.45f, 0.45f, 0.48f, 1.0f};
    t.buttonNormal          = {0.80f, 0.80f, 0.83f, 1.0f};
    t.buttonHover           = {0.70f, 0.70f, 0.75f, 1.0f};
    t.buttonPressed         = {0.60f, 0.60f, 0.65f, 1.0f};
    t.buttonText            = {0.10f, 0.10f, 0.12f, 1.0f};
    t.divider               = {0.72f, 0.72f, 0.74f, 1.0f};
    t.dividerHovered        = {0.50f, 0.50f, 0.55f, 1.0f};
    t.scrollbarTrack        = {0.82f, 0.82f, 0.84f, 0.8f};
    t.scrollbarThumb        = {0.60f, 0.60f, 0.63f, 0.9f};
    t.browserBackground     = {0.93f, 0.93f, 0.94f, 1.0f};
    t.browserRowOdd         = {0.00f, 0.00f, 0.00f, 0.04f};
    t.browserSelected       = {0.25f, 0.50f, 0.90f, 0.85f};
    t.browserSelectedText   = {1.00f, 1.00f, 1.00f, 1.0f};
    t.browserText           = {0.12f, 0.12f, 0.15f, 1.0f};
    t.folderIcon            = {0.30f, 0.55f, 0.78f, 0.7f};
    t.textPrimary           = {0.10f, 0.10f, 0.12f, 1.0f};
    t.textSecondary         = {0.40f, 0.40f, 0.43f, 1.0f};
    t.textDisabled          = {0.65f, 0.65f, 0.68f, 1.0f};
    t.contextMenuBackground = {0.97f, 0.97f, 0.98f, 1.0f};
    t.contextMenuHover      = {0.25f, 0.50f, 0.90f, 0.80f};
    t.contextMenuText       = {0.10f, 0.10f, 0.12f, 1.0f};
    t.contextMenuSeparator  = {0.78f, 0.78f, 0.80f, 1.0f};
    t.accent                = {0.20f, 0.50f, 0.90f, 1.0f};
    t.accentHover           = {0.30f, 0.60f, 1.00f, 1.0f};
    return t;
}

} // namespace orf
