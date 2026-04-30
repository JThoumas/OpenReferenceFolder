#include "collapsible_region.h"
#include "framework/docking/tab_group_node.h"

namespace orf {

CollapsibleRegion::CollapsibleRegion(Side side, Font& font)
    : m_side(side), m_font(font)
{
    m_content = std::make_unique<TabGroupNode>(font);
}

CollapsibleRegion::~CollapsibleRegion() = default;

void CollapsibleRegion::setBounds(const Rect& r) {
    m_bounds = r;
    if (m_content) m_content->layout(r);
}

void CollapsibleRegion::setCollapsed(bool v) {
    m_collapsed = v;
}

void CollapsibleRegion::toggleCollapsed() {
    m_collapsed = !m_collapsed;
}

void CollapsibleRegion::addTab(const std::string& title, std::unique_ptr<Widget> content) {
    if (m_content) m_content->addTab(title, std::move(content));
}

void CollapsibleRegion::paint(Renderer2D& renderer) {
    if (m_content && !m_collapsed) m_content->paint(renderer);
}

void CollapsibleRegion::onMouseMove(float x, float y) {
    if (m_content && !m_collapsed) m_content->handleMouseMove(x, y);
}

void CollapsibleRegion::onMousePress(float x, float y, int button) {
    if (m_content && !m_collapsed) m_content->handleMousePress(x, y, button);
}

void CollapsibleRegion::onMouseRelease(float x, float y, int button) {
    if (m_content && !m_collapsed) m_content->handleMouseRelease(x, y, button);
}

void CollapsibleRegion::onScroll(float delta) {
    // Scroll handling will be implemented later
}

} // namespace orf
