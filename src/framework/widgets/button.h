#pragma once
#include "widget.h"
#include "framework/renderer/font.h"
#include <string>
#include <functional>

namespace orf {

class Button : public Widget {
public:
    Button(const std::string& label, Font& font);

    void setLabel   (const std::string& label);
    void setOnClick (std::function<void()> callback);

    // Color scheme — all public so callers can theme them
    Color colorNormal  = {0.25f, 0.25f, 0.30f, 1.0f};
    Color colorHover   = {0.35f, 0.35f, 0.42f, 1.0f};
    Color colorPressed = {0.18f, 0.18f, 0.22f, 1.0f};
    Color colorLabel   = Color::white();

    void onPaint         (Renderer2D& renderer)  override;
    void onMouseMove     (MouseEvent& e)          override;
    void onMouseMoveGlobal(float x, float y)      override;
    void onMousePress    (MouseEvent& e)          override;
    void onMouseRelease  (MouseEvent& e)          override;

private:
    enum class State { Normal, Hovered, Pressed };

    std::string           m_label;
    Font&                 m_font;
    State                 m_state = State::Normal;
    std::function<void()> m_onClick;
};

} // namespace orf
