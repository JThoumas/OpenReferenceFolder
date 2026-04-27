#include "display_center_widget.h"
#include "framework/core/log.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <GLFW/glfw3.h>

using json = nlohmann::json;

namespace orf {

static constexpr float TAB_H    = 36.0f;
static constexpr float ITEM_H   = 32.0f;
static constexpr float PADDING  = 12.0f;

DisplayCenterWidget::DisplayCenterWidget(Font& font,
                                         ThemeManager& themes,
                                         Database& db)
    : m_font(font), m_themes(themes), m_db(db)
{
    loadWorkspaceList();

    // Scan themes directory
    if (std::filesystem::exists("themes")) {
        for (auto& e : std::filesystem::directory_iterator("themes")) {
            if (e.path().extension() == ".json")
                m_themeFiles.push_back(e.path().string());
        }
    }
}

void DisplayCenterWidget::loadWorkspaceList() {
    m_workspaceNames.clear();
    if (!std::filesystem::exists("workspaces")) return;
    for (auto& e : std::filesystem::directory_iterator("workspaces")) {
        if (e.path().extension() == ".json")
            m_workspaceNames.push_back(
                e.path().stem().string());
    }
    std::sort(m_workspaceNames.begin(), m_workspaceNames.end());
}

void DisplayCenterWidget::onResize(int, int) { markDirty(); }

void DisplayCenterWidget::paintTabBar(Renderer2D& renderer) {
    const char* tabLabels[] = {"Workspaces", "Themes", "Monitors"};
    float tabW = m_bounds.width / 3.0f;

    renderer.drawRect({m_bounds.x, m_bounds.y, m_bounds.width, TAB_H},
                       theme().tabBarBackground);

    for (int i = 0; i < 3; ++i) {
        bool active = (int)m_activeTab == i;
        float tx = m_bounds.x + i * tabW;
        renderer.drawRect({tx, m_bounds.y, tabW, TAB_H},
                           active ? theme().tabActive : theme().tabBarBackground);
        renderer.drawText(tabLabels[i],
                          tx + PADDING,
                          m_bounds.y + TAB_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font,
                          active ? theme().tabActiveText : theme().tabInactiveText);
    }
}

void DisplayCenterWidget::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    // Background
    renderer.drawRect(m_bounds, theme().panelBackground);

    paintTabBar(renderer);

    Rect contentArea = {
        m_bounds.x,
        m_bounds.y + TAB_H,
        m_bounds.width,
        m_bounds.height - TAB_H
    };
    renderer.pushClip(contentArea);

    switch (m_activeTab) {
        case DisplayCenterTab::Workspaces: paintWorkspaces(renderer); break;
        case DisplayCenterTab::Themes:     paintThemes(renderer);     break;
        case DisplayCenterTab::Monitors:   paintMonitors(renderer);   break;
    }

    renderer.popClip();
    clearDirty();
}

void DisplayCenterWidget::paintWorkspaces(Renderer2D& renderer) {
    float y = m_bounds.y + TAB_H + PADDING;

    renderer.drawText("Saved Workspaces",
                       m_bounds.x + PADDING, y + m_font.lineHeight(),
                       m_font, theme().textPrimary);
    y += m_font.lineHeight() + 8.0f;

    for (int i = 0; i < (int)m_workspaceNames.size(); ++i) {
        bool hovered  = m_hoveredItem == i;
        bool selected = m_selectedLayout == i;

        renderer.drawRect({m_bounds.x + PADDING, y,
                           m_bounds.width - PADDING * 2, ITEM_H},
                           selected ? theme().browserSelected
                           : hovered ? theme().buttonHover
                                     : theme().buttonNormal);

        renderer.drawText(m_workspaceNames[i],
                          m_bounds.x + PADDING * 2,
                          y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font,
                          selected ? theme().browserSelectedText
                                   : theme().textPrimary);

        // Delete button (far right of row)
        renderer.drawRect({m_bounds.x + m_bounds.width - PADDING - 24.0f,
                           y + 4.0f, 20.0f, ITEM_H - 8.0f},
                           {0.65f, 0.25f, 0.25f, 0.8f});
        renderer.drawText("×",
                          m_bounds.x + m_bounds.width - PADDING - 18.0f,
                          y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font, Color::white());

        y += ITEM_H + 4.0f;
    }

    // Save current button
    y += 8.0f;
    renderer.drawRect({m_bounds.x + PADDING, y,
                       180.0f, ITEM_H},
                       theme().accent);
    renderer.drawText("Save Current Layout",
                       m_bounds.x + PADDING * 2, y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                       m_font, Color::white());
}

void DisplayCenterWidget::paintThemes(Renderer2D& renderer) {
    float y = m_bounds.y + TAB_H + PADDING;

    renderer.drawText("Color Schemes",
                       m_bounds.x + PADDING, y + m_font.lineHeight(),
                       m_font, theme().textPrimary);
    y += m_font.lineHeight() + 8.0f;

    // Built-in themes
    const char* builtins[] = {"ORF Dark", "ORF Light"};
    for (auto& name : builtins) {
        bool current = (theme().name == name);
        renderer.drawRect({m_bounds.x + PADDING, y,
                           m_bounds.width - PADDING * 2, ITEM_H},
                           current ? theme().browserSelected : theme().buttonNormal);
        renderer.drawText(name,
                          m_bounds.x + PADDING * 2,
                          y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font,
                          current ? theme().browserSelectedText : theme().textPrimary);
        if (current) {
            renderer.drawText("● Active",
                              m_bounds.x + m_bounds.width - PADDING - 70.0f,
                              y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                              m_font, theme().accent);
        }
        y += ITEM_H + 4.0f;
    }

    // User theme files from themes/ directory
    for (auto& path : m_themeFiles) {
        std::string name = std::filesystem::path(path).stem().string();
        renderer.drawRect({m_bounds.x + PADDING, y,
                           m_bounds.width - PADDING * 2, ITEM_H},
                           theme().buttonNormal);
        renderer.drawText(name,
                          m_bounds.x + PADDING * 2,
                          y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font, theme().textPrimary);
        y += ITEM_H + 4.0f;
    }
}

void DisplayCenterWidget::paintMonitors(Renderer2D& renderer) {
    float y = m_bounds.y + TAB_H + PADDING;

    renderer.drawText("Monitor Configuration",
                       m_bounds.x + PADDING, y + m_font.lineHeight(),
                       m_font, theme().textPrimary);
    y += m_font.lineHeight() + 12.0f;

    // Query GLFW for connected monitors
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    for (int i = 0; i < count; ++i) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
        const char*        name = glfwGetMonitorName(monitors[i]);

        char info[128];
        snprintf(info, sizeof(info), "%s — %dx%d @ %dHz",
                 name, mode->width, mode->height, mode->refreshRate);

        renderer.drawRect({m_bounds.x + PADDING, y,
                           m_bounds.width - PADDING * 2, ITEM_H},
                           i == 0 ? theme().tabActive : theme().buttonNormal);
        renderer.drawText(info,
                          m_bounds.x + PADDING * 2,
                          y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font, theme().textPrimary);
        if (i == 0) {
            renderer.drawText("Primary",
                              m_bounds.x + m_bounds.width - PADDING - 55.0f,
                              y + ITEM_H * 0.5f + m_font.lineHeight() * 0.4f,
                              m_font, theme().accent);
        }
        y += ITEM_H + 4.0f;
    }

    y += 16.0f;
    renderer.drawText("Multi-window layout support coming in a future release.",
                       m_bounds.x + PADDING, y + m_font.lineHeight(),
                       m_font, theme().textSecondary);
}

void DisplayCenterWidget::onMousePress(MouseEvent& e) {
    // Tab bar click
    float tabW = m_bounds.width / 3.0f;
    if (e.y >= m_bounds.y && e.y < m_bounds.y + TAB_H) {
        int tab = (int)((e.x - m_bounds.x) / tabW);
        if (tab >= 0 && tab < 3) {
            m_activeTab = static_cast<DisplayCenterTab>(tab);
            markDirty();
            e.consumed = true;
            return;
        }
    }

    // Workspaces tab interactions
    if (m_activeTab == DisplayCenterTab::Workspaces) {
        float y     = m_bounds.y + TAB_H + PADDING + m_font.lineHeight() + 8.0f;
        float saveY = y + (m_workspaceNames.size()) * (ITEM_H + 4.0f) + 8.0f;

        // Check Save Current Layout button
        if (e.y >= saveY && e.y < saveY + ITEM_H
         && e.x >= m_bounds.x + PADDING
         && e.x < m_bounds.x + PADDING + 180.0f) {
            saveCurrentWorkspace("workspace_" +
                std::to_string(m_workspaceNames.size() + 1));
            loadWorkspaceList();
            markDirty();
            e.consumed = true;
            return;
        }

        // Check workspace row clicks
        for (int i = 0; i < (int)m_workspaceNames.size(); ++i) {
            float ry = y + i * (ITEM_H + 4.0f);
            if (e.y >= ry && e.y < ry + ITEM_H) {
                // Delete button
                if (e.x >= m_bounds.x + m_bounds.width - PADDING - 24.0f) {
                    deleteWorkspace(m_workspaceNames[i]);
                    loadWorkspaceList();
                    markDirty();
                } else {
                    m_selectedLayout = i;
                    applyWorkspace(m_workspaceNames[i]);
                    markDirty();
                }
                e.consumed = true;
                return;
            }
        }
    }

    // Themes tab interactions
    if (m_activeTab == DisplayCenterTab::Themes) {
        float y = m_bounds.y + TAB_H + PADDING + m_font.lineHeight() + 8.0f;

        if (e.y >= y && e.y < y + ITEM_H) {
            ThemeManager::get().resetToDefault();
            markDirty(); e.consumed = true; return;
        }
        y += ITEM_H + 4.0f;
        if (e.y >= y && e.y < y + ITEM_H) {
            Theme light = ThemeManager::lightTheme();
            ThemeManager::get().current() = light;
            ThemeManager::get().notifyChanged();
            markDirty(); e.consumed = true; return;
        }
        y += ITEM_H + 4.0f;
        for (int i = 0; i < (int)m_themeFiles.size(); ++i) {
            if (e.y >= y && e.y < y + ITEM_H) {
                ThemeManager::get().loadFromFile(m_themeFiles[i]);
                markDirty(); e.consumed = true; return;
            }
            y += ITEM_H + 4.0f;
        }
    }
}

void DisplayCenterWidget::onMouseMove(MouseEvent& e) {
    float y = m_bounds.y + TAB_H + PADDING + m_font.lineHeight() + 8.0f;
    int hovered = -1;
    for (int i = 0; i < (int)m_workspaceNames.size(); ++i) {
        float ry = y + i * (ITEM_H + 4.0f);
        if (e.y >= ry && e.y < ry + ITEM_H) { hovered = i; break; }
    }
    if (hovered != m_hoveredItem) { m_hoveredItem = hovered; markDirty(); }
}

void DisplayCenterWidget::saveCurrentWorkspace(const std::string& name) {
    std::filesystem::create_directories("workspaces");
    if (m_onSaveWorkspace) m_onSaveWorkspace(name);
    LOG_INFO("DisplayCenter: saved workspace '" << name << "'");
}

void DisplayCenterWidget::applyWorkspace(const std::string& name) {
    if (m_onApplyWorkspace) m_onApplyWorkspace(name);
    LOG_INFO("DisplayCenter: applying workspace '" << name << "'");
}

void DisplayCenterWidget::deleteWorkspace(const std::string& name) {
    if (m_onDeleteWorkspace) m_onDeleteWorkspace(name);
    std::filesystem::remove("workspaces/" + name + ".json");
    LOG_INFO("DisplayCenter: deleted workspace '" << name << "'");
}

} // namespace orf
