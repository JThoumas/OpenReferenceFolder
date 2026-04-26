#include "floating_window.h"
#include "framework/core/log.h"

namespace orf {

FloatingWindow::FloatingWindow(std::unique_ptr<PanelNode> panel,
                               GLFWwindow* sharedContext,
                               Font& font,
                               int x, int y, int w, int h)
    : m_panel(std::move(panel))
    , m_font(font)
{
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    m_window = glfwCreateWindow(w, h, m_panel->title().c_str(), nullptr, sharedContext);
    if (!m_window) {
        LOG_ERROR("Failed to create floating GLFW window");
        return;
    }
    glfwSetWindowPos(m_window, x, y);

    // Make context current to init renderer
    GLFWwindow* previousContext = glfwGetCurrentContext();
    glfwMakeContextCurrent(m_window);
    
    m_renderer.init(w, h);
    
    glfwMakeContextCurrent(previousContext);
    
    m_panel->layout({0, 0, (float)w, (float)h});
}

FloatingWindow::~FloatingWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
}

bool FloatingWindow::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void FloatingWindow::update() {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    m_renderer.setViewportSize(w, h);
    m_panel->layout({0, 0, (float)w, (float)h});
}

void FloatingWindow::paint() {
    GLFWwindow* previousContext = glfwGetCurrentContext();
    glfwMakeContextCurrent(m_window);

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_renderer.begin();
    m_panel->paint(m_renderer);
    m_renderer.end();

    glfwSwapBuffers(m_window);
    glfwMakeContextCurrent(previousContext);
}

std::unique_ptr<PanelNode> FloatingWindow::detachPanel() {
    return std::move(m_panel);
}

} // namespace orf
