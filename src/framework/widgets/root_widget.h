#pragma once
#include "widget.h"
#include "framework/renderer/font.h"

namespace orf {

// RootWidget is the top of the widget tree.
// It maps to the GLFW window and receives all input from Application.
class RootWidget : public Widget {
public:
    // font is stored for widgets that need to render text
    explicit RootWidget(Font& font) : m_font(font) {}

    void resize(int w, int h);

    // Called by Application each frame
    void dispatchMouseMove  (float x, float y);
    void dispatchMousePress (float x, float y, int button);
    void dispatchMouseRelease(float x, float y, int button);
    void dispatchKey        (int key, int action);

    // Paint the entire tree if dirty
    void paintIfDirty(Renderer2D& renderer);

    Font& font() { return m_font; }

private:
    Font& m_font;
};

} // namespace orf
