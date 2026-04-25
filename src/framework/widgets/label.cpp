#include "label.h"
#include "root_widget.h"

namespace orf {

void Label::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    RootWidget* root = getRootWidget();
    if (root) {
        renderer.drawText(m_text, m_bounds.x, m_bounds.y, root->font(), m_color);
    }

    paintChildren(renderer);
    clearDirty();
}

} // namespace orf
