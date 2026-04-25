#include "panel.h"

namespace orf {

void Panel::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    if (m_backgroundColor.a > 0.0f) {
        renderer.drawRect(m_bounds, m_backgroundColor);
    }

    paintChildren(renderer);
    clearDirty();
}

} // namespace orf
