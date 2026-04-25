#include "label.h"
#include "root_widget.h"

namespace orf {

void Label::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    // Traverse up to find the RootWidget for the font reference
    Widget* root = this;
    while (root->parent()) {
        root = root->parent();
    }

    RootWidget* rootWidget = dynamic_cast<RootWidget*>(root);
    if (rootWidget) {
        // Render text at the top-left of the label bounds.
        // drawText uses baseline, but our Font implementation (likely) 
        // handles the y offset internally or expects it at top-left.
        // In most of our previous renderer tests, drawText(text, x, y, font, color) 
        // drew starting from x,y.
        renderer.drawText(m_text, m_bounds.x, m_bounds.y, rootWidget->font(), m_color);
    }

    paintChildren(renderer);
    clearDirty();
}

} // namespace orf
