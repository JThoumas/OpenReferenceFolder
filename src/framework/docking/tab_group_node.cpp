#include "tab_group_node.h"
#include <algorithm>

namespace orf {

TabGroupNode::TabGroupNode(Font& font) : m_font(font) {}

void TabGroupNode::addTab(const std::string& title,
                           std::unique_ptr<Widget> content) {
    m_tabs.push_back({title, std::move(content)});
}

void TabGroupNode::setActiveTab(int index) {
    m_activeTab = std::clamp(index, 0, (int)m_tabs.size() - 1);
}

Rect TabGroupNode::tabRect(int index) const {
    float tabW = std::max(TAB_MIN_WIDTH, m_bounds.width / (float)m_tabs.size());
    return {
        m_bounds.x + index * tabW,
        m_bounds.y,
        tabW,
        TAB_BAR_HEIGHT
    };
}

int TabGroupNode::tabAt(float x, float y) const {
    if (m_tabs.empty()) return -1;
    for (int i = 0; i < (int)m_tabs.size(); ++i) {
        Rect r = tabRect(i);
        if (x >= r.x && x < r.x + r.width && y >= r.y && y < r.y + r.height)
            return i;
    }
    return -1;
}

void TabGroupNode::layout(const Rect& bounds) {
    m_bounds = bounds;
    Rect contentBounds = {
        bounds.x,
        bounds.y + TAB_BAR_HEIGHT,
        bounds.width,
        bounds.height - TAB_BAR_HEIGHT
    };
    // Layout all tabs — only the active one is visible
    for (auto& tab : m_tabs) {
        tab.content->setBounds(contentBounds);
        tab.content->onLayout();
    }
}

void TabGroupNode::paint(Renderer2D& renderer) {
    // Tab bar background
    renderer.drawRect(
        {m_bounds.x, m_bounds.y, m_bounds.width, TAB_BAR_HEIGHT},
        {0.12f, 0.12f, 0.14f, 1.0f}
    );

    // Individual tabs
    if (!m_tabs.empty()) {
        for (int i = 0; i < (int)m_tabs.size(); ++i) {
            Rect tr   = tabRect(i);
            bool active = (i == m_activeTab);
            renderer.drawRect(tr, active
                ? Color{0.20f, 0.20f, 0.24f, 1.0f}
                : Color{0.14f, 0.14f, 0.16f, 1.0f});
            renderer.drawText(
                m_tabs[i].title,
                tr.x + 8.0f,
                tr.y + m_font.lineHeight() + 4.0f,
                m_font,
                active ? Color::white() : Color{0.6f, 0.6f, 0.6f, 1.0f}
            );
        }

        // Active content
        if (m_activeTab < (int)m_tabs.size()) {
            Rect contentBounds = {
                m_bounds.x,
                m_bounds.y + TAB_BAR_HEIGHT,
                m_bounds.width,
                m_bounds.height - TAB_BAR_HEIGHT
            };
            renderer.pushClip(contentBounds);
            m_tabs[m_activeTab].content->onPaint(renderer);
            renderer.popClip();
        }
    }
}

bool TabGroupNode::handleMousePress(float x, float y, int button) {
    int idx = tabAt(x, y);
    if (idx >= 0) {
        setActiveTab(idx);
        return true;
    }
    return false;
}

nlohmann::json TabGroupNode::serialize() const {
    nlohmann::json j;
    j["type"]      = "tabgroup";
    j["activeTab"] = m_activeTab;
    std::vector<std::string> tabTitles;
    for (const auto& tab : m_tabs) tabTitles.push_back(tab.title);
    j["tabs"] = tabTitles;
    return j;
}

} // namespace orf
