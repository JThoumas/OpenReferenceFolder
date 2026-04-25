#pragma once
#include "widget.h"

namespace orf {

class Panel : public Widget {
public:
    Panel(const Color& backgroundColor = Color::transparent())
        : m_backgroundColor(backgroundColor) {}

    void setBackgroundColor(const Color& color) { m_backgroundColor = color; markDirty(); }
    Color backgroundColor() const { return m_backgroundColor; }

    void onPaint(Renderer2D& renderer) override;

private:
    Color m_backgroundColor;
};

} // namespace orf
