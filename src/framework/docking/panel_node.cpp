#include "panel_node.h"
#include "framework/theme/theme_manager.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace orf {

PanelNode::PanelNode(const std::string& title,
                     std::unique_ptr<Widget> content,
                     Font& font)
    : m_title(title)
    , m_content(std::move(content))
    , m_font(font)
{}

void PanelNode::layout(const Rect& bounds) {
    m_bounds = bounds;
    // Content area sits below the title bar
    Rect contentBounds = {
        bounds.x,
        bounds.y + TITLE_BAR_HEIGHT,
        bounds.width,
        bounds.height - TITLE_BAR_HEIGHT
    };
    m_content->setBounds(contentBounds);
    m_content->onLayout();
}

void PanelNode::markDirty() {
    m_content->markDirty();
}

void PanelNode::paint(Renderer2D& renderer) {
    // Title bar background
    renderer.drawRect(
        {m_bounds.x, m_bounds.y, m_bounds.width, TITLE_BAR_HEIGHT},
        theme().titleBarBackground
    );
    // Title text
    renderer.drawText(
        m_title,
        m_bounds.x + 8.0f,
        m_bounds.y + m_font.lineHeight(),
        m_font,
        theme().titleBarText
    );
    // Content area
    renderer.pushClip({
        m_bounds.x,
        m_bounds.y + TITLE_BAR_HEIGHT,
        m_bounds.width,
        m_bounds.height - TITLE_BAR_HEIGHT
    });
    m_content->onPaint(renderer);
    renderer.popClip();
}

bool PanelNode::titleBarContains(float x, float y) const {
    return x >= m_bounds.x
        && x <  m_bounds.x + m_bounds.width
        && y >= m_bounds.y
        && y <  m_bounds.y + TITLE_BAR_HEIGHT;
}

bool PanelNode::handleMousePress(float x, float y, int button) {
    if (button == 0 && titleBarContains(x, y)) {
        double currentTime = glfwGetTime();
        if (currentTime - m_lastPressTime < 0.3) {
            if (m_onDoubleClick) m_onDoubleClick(this);
            m_dragging = false;
            return true;
        }
        m_lastPressTime = currentTime;

        m_dragging = true;
        m_dragOffX = x - m_bounds.x;
        m_dragOffY = y - m_bounds.y;
        return true;
    }
    return false;
}

bool PanelNode::handleMouseRelease(float x, float y, int button) {
    if (m_dragging) { m_dragging = false; return true; }
    return false;
}

bool PanelNode::handleMouseMove(float x, float y) {
    // Dragging behaviour is handled by DockManager in Day 3
    return m_dragging;
}

nlohmann::json PanelNode::serialize() const {
    nlohmann::json j;
    j["type"]  = "panel";
    j["title"] = m_title;
    return j;
}

} // namespace orf
