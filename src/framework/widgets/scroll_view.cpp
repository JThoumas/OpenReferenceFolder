#include "scroll_view.h"
#include "framework/theme/theme_manager.h"
#include <algorithm>

namespace orf {

ScrollView::ScrollView(const Color& background)
    : m_background(background) {}

void ScrollView::setContentHeight(float h) {
    m_contentHeight = h;
    markDirty();
}

void ScrollView::onScroll(float yOffset) {
    float maxOffset = std::max(0.0f, m_contentHeight - m_bounds.height);
    m_scrollOffset  = std::clamp(m_scrollOffset - yOffset * m_scrollSpeed,
                                  0.0f, maxOffset);
    markDirty();
}

void ScrollView::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    if (m_background.a > 0.0f) {
        renderer.drawRect(m_bounds, m_background);
    } else {
        renderer.drawRect(m_bounds, theme().windowBackground);
    }

    // Clip children to the scroll view's bounds
    renderer.pushClip(m_bounds);
    for (auto& child : m_children) {
        if (!child->isVisible()) continue;
        
        // Offset child bounds by scroll amount before painting
        Rect orig = child->bounds();
        Rect shifted = {
            orig.x,
            orig.y - m_scrollOffset,
            orig.width,
            orig.height
        };
        child->setBounds(shifted);
        child->onPaint(renderer);
        child->setBounds(orig); // restore
    }
    renderer.popClip();

    paintScrollbar(renderer);
    clearDirty();
}

void ScrollView::paintScrollbar(Renderer2D& renderer) {
    if (m_contentHeight <= m_bounds.height) return; // no scrollbar needed

    float trackW  = 8.0f;
    float trackX  = m_bounds.x + m_bounds.width - trackW;
    float trackH  = m_bounds.height;

    // Track
    renderer.drawRect({trackX, m_bounds.y, trackW, trackH},
                       theme().scrollbarTrack);

    // Thumb — size proportional to visible/total ratio
    float thumbH   = (m_bounds.height / m_contentHeight) * trackH;
    float maxOffset = m_contentHeight - m_bounds.height;
    float thumbY   = m_bounds.y + (m_scrollOffset / maxOffset) * (trackH - thumbH);

    renderer.drawRect({trackX, thumbY, trackW, thumbH},
                       theme().scrollbarThumb);
}

} // namespace orf
