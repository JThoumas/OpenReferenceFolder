#pragma once
#include "widget.h"

namespace orf {

class ScrollView : public Widget {
public:
    explicit ScrollView(const Color& background = {0.15f, 0.15f, 0.18f, 1.0f});

    // Content height is the total height of all children combined
    void  setContentHeight(float h);
    float scrollOffset() const { return m_scrollOffset; }

    void onPaint (Renderer2D& renderer) override;
    void onScroll(float yOffset)        override;

private:
    Color m_background;
    float m_contentHeight = 0.0f;
    float m_scrollOffset  = 0.0f;
    float m_scrollSpeed   = 20.0f;

    // Draws the scrollbar track and thumb
    void paintScrollbar(Renderer2D& renderer);
};

} // namespace orf
