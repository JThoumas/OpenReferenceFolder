#include "collapsible_region.h"
#include "framework/docking/tab_group_node.h"
#include "framework/theme/theme_manager.h"

namespace orf {

static constexpr float TOGGLE_BTN_SIZE  = 20.0f;

CollapsibleRegion::CollapsibleRegion(Side side, Font& font)
    : m_side(side), m_font(font)
{
    m_content = std::make_unique<TabGroupNode>(font);
}

CollapsibleRegion::~CollapsibleRegion() = default;

void CollapsibleRegion::setBounds(const Rect& r) {
    m_bounds = r;
    if (!m_collapsed && m_content) {
        // Give the content the full bounds minus toggle button strip (handled by DockNode::layout)
        m_content->layout(m_bounds);
    }
}

void CollapsibleRegion::setCollapsed(bool v) {
    m_collapsed = v;
}

void CollapsibleRegion::toggleCollapsed() {
    m_collapsed = !m_collapsed;
}

void CollapsibleRegion::addTab(const std::string& title,
                                std::unique_ptr<Widget> content) {
    if (m_content)
        m_content->addTab(title, std::move(content));
}

// ---- Toggle button geometry ----

Rect CollapsibleRegion::toggleButtonRect() const {
    float cx = m_bounds.x + m_bounds.width  * 0.5f - TOGGLE_BTN_SIZE * 0.5f;
    float cy = m_bounds.y + m_bounds.height * 0.5f - TOGGLE_BTN_SIZE * 0.5f;

    switch (m_side) {
        case Side::Left:
            // Button sits at the right edge of the skinny bar
            return {m_bounds.x + m_bounds.width - TOGGLE_BTN_SIZE,
                    cy, TOGGLE_BTN_SIZE, TOGGLE_BTN_SIZE};
        case Side::Right:
            return {m_bounds.x, cy, TOGGLE_BTN_SIZE, TOGGLE_BTN_SIZE};
        case Side::Bottom:
            return {cx, m_bounds.y, TOGGLE_BTN_SIZE, TOGGLE_BTN_SIZE};
    }
    return {};
}

bool CollapsibleRegion::toggleButtonContains(float x, float y) const {
    Rect r = toggleButtonRect();
    return x >= r.x && x < r.x + r.width
        && y >= r.y && y < r.y + r.height;
}

const char* CollapsibleRegion::toggleLabel() const {
    switch (m_side) {
        case Side::Left:   return m_collapsed ? "›" : "‹";
        case Side::Right:  return m_collapsed ? "‹" : "›";
        case Side::Bottom: return m_collapsed ? "ˆ" : "ˇ";
    }
    return "";
}

// ---- Paint ----

void CollapsibleRegion::paint(Renderer2D& renderer) {
    if (m_collapsed)
        paintCollapsed(renderer);
    else
        paintExpanded(renderer);
}

void CollapsibleRegion::paintCollapsed(Renderer2D& renderer) {
    // Skinny bar background
    renderer.drawRect(m_bounds, theme().panelBackground);

    // Border line on the interior edge
    switch (m_side) {
        case Side::Left:
            renderer.drawRect({m_bounds.x + m_bounds.width - 1.0f,
                               m_bounds.y, 1.0f, m_bounds.height},
                               theme().panelBorder);
            break;
        case Side::Right:
            renderer.drawRect({m_bounds.x, m_bounds.y,
                               1.0f, m_bounds.height},
                               theme().panelBorder);
            break;
        case Side::Bottom:
            renderer.drawRect({m_bounds.x, m_bounds.y,
                               m_bounds.width, 1.0f},
                               theme().panelBorder);
            break;
    }

    // Toggle button
    Rect btn = toggleButtonRect();
    renderer.drawRect(btn, m_btnHovered
        ? theme().buttonHover
        : theme().buttonNormal);
    renderer.drawText(toggleLabel(),
                       btn.x + 3.0f,
                       btn.y + btn.height * 0.5f + m_font.lineHeight() * 0.4f,
                       m_font, theme().textPrimary);
}

void CollapsibleRegion::paintExpanded(Renderer2D& renderer) {
    renderer.drawRect(m_bounds, theme().panelBackground);

    // Interior border
    switch (m_side) {
        case Side::Left:
            renderer.drawRect({m_bounds.x + m_bounds.width - 1.0f,
                               m_bounds.y, 1.0f, m_bounds.height},
                               theme().panelBorder);
            break;
        case Side::Right:
            renderer.drawRect({m_bounds.x, m_bounds.y,
                               1.0f, m_bounds.height},
                               theme().panelBorder);
            break;
        case Side::Bottom:
            renderer.drawRect({m_bounds.x, m_bounds.y,
                               m_bounds.width, 1.0f},
                               theme().panelBorder);
            break;
    }

    // Content (TabGroupNode)
    if (m_content)
        m_content->paint(renderer);

    // Toggle button — sits on the interior edge of the expanded region
    Rect btn = toggleButtonRect();
    renderer.drawRect(btn, m_btnHovered
        ? theme().buttonHover
        : theme().buttonNormal);
    renderer.drawText(toggleLabel(),
                       btn.x + 3.0f,
                       btn.y + btn.height * 0.5f + m_font.lineHeight() * 0.4f,
                       m_font, theme().textSecondary);
}

// ---- Input ----

void CollapsibleRegion::onMousePress(float x, float y, int button) {
    if (button != 0) return;
    if (x < m_bounds.x || x >= m_bounds.x + m_bounds.width) return;
    if (y < m_bounds.y || y >= m_bounds.y + m_bounds.height) return;

    if (toggleButtonContains(x, y)) {
        toggleCollapsed();
        // RegionManager sees the state change and calls recalculateBounds()
        return;
    }

    if (!m_collapsed && m_content)
        m_content->handleMousePress(x, y, button);
}

void CollapsibleRegion::onMouseRelease(float x, float y, int button) {
    if (!m_collapsed && m_content)
        m_content->handleMouseRelease(x, y, button);
}

void CollapsibleRegion::onMouseMove(float x, float y) {
    m_btnHovered = toggleButtonContains(x, y);

    if (!m_collapsed && m_content)
        m_content->handleMouseMove(x, y);
}

void CollapsibleRegion::onScroll(float delta) {
    // Scroll handling will be implemented later
}

} // namespace orf
