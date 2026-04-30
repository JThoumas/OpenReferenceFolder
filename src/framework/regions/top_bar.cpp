#include "top_bar.h"
#include "framework/theme/theme_manager.h"

namespace orf {

TopBar::TopBar(Font& font) : m_font(font) {}

void TopBar::setBounds(const Rect& r) {
    m_bounds = r;
}

void TopBar::addMenu(Menu menu) {
    m_menus.push_back(std::move(menu));
}

void TopBar::paint(Renderer2D& renderer) {
    // Basic painting stub
    renderer.drawRect(m_bounds, theme().titleBarBackground);
}

void TopBar::onMouseMove(float x, float y) {
}

void TopBar::onMousePress(float x, float y, int button) {
}

void TopBar::onMouseRelease(float x, float y, int button) {
}

void TopBar::dismissMenu() {
}

} // namespace orf
