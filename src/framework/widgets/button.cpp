#include "button.h"

namespace orf {

Button::Button(const std::string& label, Font& font)
    : m_label(label), m_font(font) {}

void Button::setLabel(const std::string& label) {
    m_label = label;
    markDirty();
}

void Button::setOnClick(std::function<void()> callback) {
    m_onClick = std::move(callback);
}

void Button::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    // Pick fill color based on state
    const Color& fill = (m_state == State::Pressed) ? colorPressed
                      : (m_state == State::Hovered)  ? colorHover
                                                      : colorNormal;
    renderer.drawRect(m_bounds, fill);

    // Center the label text within the button bounds
    // Center properly based on lineHeight
    float textX = m_bounds.x + 8.0f;
    float textY = m_bounds.y + (m_bounds.height * 0.5f) + (m_font.lineHeight() * 0.3f);
    renderer.drawText(m_label, textX, textY, m_font, colorLabel);

    paintChildren(renderer);
    clearDirty();
}

void Button::onMouseMove(MouseEvent& e) {
    // If the cursor is inside us but we got here, we're hovered
    if (m_state != State::Pressed) {
        if (m_state != State::Hovered) {
            m_state = State::Hovered;
            markDirty();
        }
    }
    e.consumed = true;
}

void Button::onMouseMoveGlobal(float x, float y) {
    Widget::onMouseMoveGlobal(x, y);
    if (!containsPoint(x, y) && m_state == State::Hovered) {
        m_state = State::Normal;
        markDirty();
    }
}

void Button::onMousePress(MouseEvent& e) {
    if (e.button == 0) { // left button
        m_state = State::Pressed;
        markDirty();
        e.consumed = true;
    }
}

void Button::onMouseRelease(MouseEvent& e) {
    if (e.button == 0 && m_state == State::Pressed) {
        m_state = containsPoint(e.x, e.y) ? State::Hovered : State::Normal;
        markDirty();
        if (m_onClick && containsPoint(e.x, e.y)) {
            m_onClick();
        }
        e.consumed = true;
    }
}

} // namespace orf
