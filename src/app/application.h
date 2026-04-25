#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"
#include "framework/widgets/root_widget.h"
#include <memory>

namespace orf {

class Application {
public:
    Application()  = default;
    ~Application() = default;

    // Delete copy constructor and assignment operator
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

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
    void onScroll(double yOffset);

    GLFWwindow* m_window   = nullptr;
    Renderer2D  m_renderer;
    Font        m_font;

    std::unique_ptr<RootWidget> m_rootWidget;

    int m_fbWidth  = 0;
    int m_fbHeight = 0;
};

} // namespace orf
