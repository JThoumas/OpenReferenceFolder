#pragma once
#include "widget.h"
#include "framework/renderer/font.h"
#include "framework/theme/theme_manager.h"
#include <string>

namespace orf {

class Label : public Widget {
public:
    Label(const std::string& text, Font& font, const std::optional<Color>& color = std::nullopt)
        : m_text(text), m_font(font), m_color(color) {}

    void setText(const std::string& text) { m_text = text; markDirty(); }
    const std::string& text() const { return m_text; }

    void setColor(const Color& color) { m_color = color; markDirty(); }
    void resetColor() { m_color = std::nullopt; markDirty(); }
    Color color() const { return m_color.value_or(theme().textPrimary); }

    void onPaint(Renderer2D& renderer) override;

private:
    std::string m_text;
    Font&       m_font;
    std::optional<Color> m_color;
};

} // namespace orf
