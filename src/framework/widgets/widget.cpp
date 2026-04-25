#include "widget.h"
#include "root_widget.h"

namespace orf {

void Widget::addChild(std::unique_ptr<Widget> child) {
    child->m_parent = this;
    m_children.push_back(std::move(child));
    markDirty();
}

void Widget::removeAllChildren() {
    m_children.clear();
    markDirty();
}

RootWidget* Widget::getRootWidget() {
    Widget* root = this;
    while (root->m_parent) {
        root = root->m_parent;
    }
    return dynamic_cast<RootWidget*>(root);
}

void Widget::setBounds(const Rect& r) {
    m_bounds = r;
    markDirty();
}

bool Widget::containsPoint(float x, float y) const {
    return x >= m_bounds.x
        && x <  m_bounds.x + m_bounds.width
        && y >= m_bounds.y
        && y <  m_bounds.y + m_bounds.height;
}

void Widget::markDirty() {
    if (m_dirty) return;
    m_dirty = true;
    // Propagate up so the root knows a repaint is needed
    if (m_parent) m_parent->markDirty();
}

void Widget::clearDirty() {
    m_dirty = false;
}

// Default implementations — subclasses override what they need

void Widget::onLayout() {
    // Default: do nothing. Subclasses override to position children.
}

void Widget::onPaint(Renderer2D& renderer) {
    // Default: just paint children
    if (!m_visible) return;
    paintChildren(renderer);
    clearDirty();
}

void Widget::paintChildren(Renderer2D& renderer) {
    for (auto& child : m_children) {
        if (child->isVisible()) {
            // Clip each child to its own bounds
            renderer.pushClip(child->bounds());
            child->onPaint(renderer);
            renderer.popClip();
        }
    }
}

void Widget::routeMouseEvent(MouseEvent& e,
                              void (Widget::*handler)(MouseEvent&)) {
    // Route to the deepest child that contains the point (back-to-front)
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        auto& child = *it;
        if (!child->isVisible()) continue;
        if (child->containsPoint(e.x, e.y)) {
            child->routeMouseEvent(e, handler);
            if (e.consumed) return;
        }
    }
    // If no child consumed it, handle it here
    (this->*handler)(e);
}

void Widget::onMouseMove(MouseEvent&)    {}

void Widget::onMouseMoveGlobal(float x, float y) {
    for (auto& child : m_children)
        child->onMouseMoveGlobal(x, y);
}

void Widget::onMousePress(MouseEvent&)   {}
void Widget::onMouseRelease(MouseEvent&) {}

void Widget::onScroll(float yOffset) {
    if (m_parent) m_parent->onScroll(yOffset);
}

void Widget::onKey(KeyEvent&)            {}
void Widget::onResize(int, int)          {}

} // namespace orf
