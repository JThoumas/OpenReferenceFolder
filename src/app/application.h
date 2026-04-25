#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"

namespace orf {

class Widget; // forward declare

class Application {
public:
    Application()  = default;
    ~Application() = default;

    bool init(int width, int height, const char* title);
    void run();
    void shutdown();

    Renderer2D& renderer() { return m_renderer; }
    Font&       font()     { return m_font; }

private:
    void onFramebufferResize(int w, int h);
    void onKey(int key, int action);
    void onMouseMove(double x, double y);
    void onMouseButton(int button, int action);

    GLFWwindow* m_window   = nullptr;
    Renderer2D  m_renderer;
    Font        m_font;

    int m_fbWidth  = 0;
    int m_fbHeight = 0;
};

} // namespace orf
