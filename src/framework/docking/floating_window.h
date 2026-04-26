#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"
#include "panel_node.h"
#include <memory>

namespace orf {

class FloatingWindow {
public:
    FloatingWindow(std::unique_ptr<PanelNode> panel,
                   GLFWwindow* sharedContext,
                   Font& font,
                   int x, int y, int w, int h);
    ~FloatingWindow();

    bool shouldClose() const;
    void update();
    void paint();

    // Returns the panel when the window is closed — for re-docking
    std::unique_ptr<PanelNode> detachPanel();

    GLFWwindow*    glfwWindow() const { return m_window; }
    PanelNode*     panel()      const { return m_panel.get(); }

private:
    GLFWwindow*              m_window = nullptr;
    Renderer2D               m_renderer;
    std::unique_ptr<PanelNode> m_panel;
    Font&                    m_font;
};

} // namespace orf
