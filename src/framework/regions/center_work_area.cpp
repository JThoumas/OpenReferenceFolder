#include "center_work_area.h"
#include "framework/docking/tab_group_node.h"

namespace orf {

CenterWorkArea::CenterWorkArea(Font& font)
    : m_font(font)
{
    m_tabs = std::make_unique<TabGroupNode>(font);
}

CenterWorkArea::~CenterWorkArea() = default;

void CenterWorkArea::setBounds(const Rect& r) {
    m_bounds = r;
    if (m_tabs) m_tabs->layout(r);
}

void CenterWorkArea::addTab(const std::string& title,
                             std::unique_ptr<Widget> content) {
    m_tabs->addTab(title, std::move(content));
    m_tabs->setActiveTab(m_tabs->tabCount() - 1);
}

bool CenterWorkArea::focusTab(const std::string& title) {
    for (int i = 0; i < m_tabs->tabCount(); ++i) {
        if (m_tabs->tabTitle(i) == title) {
            m_tabs->setActiveTab(i);
            return true;
        }
    }
    return false;
}

bool CenterWorkArea::closeTab(const std::string& title) {
    return m_tabs->removeTab(title);
}

void CenterWorkArea::paint(Renderer2D& renderer) {
    if (m_tabs) m_tabs->paint(renderer);
}

void CenterWorkArea::onMouseMove(float x, float y) {
    if (m_tabs) m_tabs->handleMouseMove(x, y);
}

void CenterWorkArea::onMousePress(float x, float y, int button) {
    if (m_tabs) m_tabs->handleMousePress(x, y, button);
}

void CenterWorkArea::onMouseRelease(float x, float y, int button) {
    if (m_tabs) m_tabs->handleMouseRelease(x, y, button);
}

void CenterWorkArea::onScroll(float delta) {
    // Scroll handling to active tab's ScrollView will be implemented later
}

void CenterWorkArea::onKey(int key, int action) {
}

} // namespace orf
