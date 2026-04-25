#pragma once
#include "widget.h"
#include <string>

namespace orf {

class Label : public Widget {
public:
    Label(const std::string& text = "", const Color& color = Color::white())
        : m_text(text), m_color(color) {}

    void setText(const std::string& text) { m_text = text; markDirty(); }
    const std::string& text() const { return m_text; }

    void setColor(const Color& color) { m_color = color; markDirty(); }
    Color color() const { return m_color; }

    void onPaint(Renderer2D& renderer) override;

private:
    std::string m_text;
    Color       m_color;
};

} // namespace orf
