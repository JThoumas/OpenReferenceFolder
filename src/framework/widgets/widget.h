#pragma once
#include "framework/renderer/renderer2d.h"
#include <vector>
#include <memory>
#include <functional>

namespace orf {

// Input event types — expand as needed in later phases
struct MouseEvent {
    float x, y;           // position in logical pixels
    int   button;         // GLFW_MOUSE_BUTTON_LEFT etc., -1 for move
    int   action;         // GLFW_PRESS, GLFW_RELEASE, -1 for move
    bool  consumed = false;
};

struct KeyEvent {
    int  key;
    int  action;          // GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
    bool consumed = false;
};

class RootWidget;

class Widget {
public:
    Widget()          = default;
    virtual ~Widget() = default;

    // Non-copyable — widgets own their children
    Widget(const Widget&)            = delete;
    Widget& operator=(const Widget&) = delete;

    // --- Tree ---
    void addChild(std::unique_ptr<Widget> child);
    void removeAllChildren();

    Widget*              parent()   const { return m_parent; }
    RootWidget*          getRootWidget();
    const std::vector<std::unique_ptr<Widget>>& children() const { return m_children; }

    // --- Layout ---
    Rect        bounds()    const { return m_bounds; }
    void        setBounds(const Rect& r);
    bool        containsPoint(float x, float y) const;

    // --- Dirty tracking ---
    bool        isDirty()   const { return m_dirty; }
    void        markDirty();       // marks self + propagates up
    void        clearDirty();      // clears self (not children)

    // --- Visibility ---
    bool        isVisible() const { return m_visible; }
    void        setVisible(bool v) { m_visible = v; markDirty(); }

    // --- Virtual interface — override in subclasses ---
    virtual void onLayout();                        // compute children bounds
    virtual void onPaint(Renderer2D& renderer);     // draw self, then children
    virtual void onMouseMove(MouseEvent& e);
    virtual void onMouseMoveGlobal(float x, float y); // Called on every move to reset states like hover
    virtual void onMousePress(MouseEvent& e);
    virtual void onMouseRelease(MouseEvent& e);
    virtual void onKey(KeyEvent& e);
    virtual void onResize(int w, int h);            // called when root resizes

protected:
    // Helpers for subclass paint implementations
    void paintChildren(Renderer2D& renderer);
    void routeMouseEvent(MouseEvent& e,
                         void (Widget::*handler)(MouseEvent&));

    Rect    m_bounds{};
    bool    m_dirty   = true;   // true on creation — needs first paint
    bool    m_visible = true;
    Widget* m_parent  = nullptr;

    std::vector<std::unique_ptr<Widget>> m_children;
};

} // namespace orf
