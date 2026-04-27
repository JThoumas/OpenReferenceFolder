#include "label.h"

namespace orf {

void Label::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    renderer.drawText(m_text, m_bounds.x, m_bounds.y + m_font.lineHeight(), m_font, color());

    paintChildren(renderer);
    clearDirty();
}

} // namespace orf
