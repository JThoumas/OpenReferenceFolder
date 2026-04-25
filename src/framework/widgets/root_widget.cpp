#include "root_widget.h"

namespace orf {

void RootWidget::resize(int w, int h) {
    setBounds({0, 0, (float)w, (float)h});
    onResize(w, h);
    // Propagate resize to all children
    for (auto& child : m_children)
        child->onResize(w, h);
}

void RootWidget::dispatchMouseMove(float x, float y) {
    MouseEvent e{x, y, -1, -1};
    routeMouseEvent(e, &Widget::onMouseMove);
}

void RootWidget::dispatchMousePress(float x, float y, int button) {
    MouseEvent e{x, y, button, 0}; // 0 = GLFW_PRESS
    routeMouseEvent(e, &Widget::onMousePress);
}

void RootWidget::dispatchMouseRelease(float x, float y, int button) {
    MouseEvent e{x, y, button, 1}; // 1 = GLFW_RELEASE
    routeMouseEvent(e, &Widget::onMouseRelease);
}

void RootWidget::dispatchKey(int key, int action) {
    KeyEvent e{key, action};
    // Keyboard goes to root for now — focus system comes in Phase 4
    onKey(e);
}

void RootWidget::paintIfDirty(Renderer2D& renderer) {
    if (!m_dirty) return;
    onPaint(renderer);
}

} // namespace orf
