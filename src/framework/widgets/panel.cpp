#include "panel.h"
#include "framework/theme/theme_manager.h"

namespace orf {

void Panel::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    if (m_backgroundColor.a > 0.0f) {
        renderer.drawRect(m_bounds, m_backgroundColor);
    } else {
        renderer.drawRect(m_bounds, theme().panelBackground);
    }

    paintChildren(renderer);
    clearDirty();
}

} // namespace orf
