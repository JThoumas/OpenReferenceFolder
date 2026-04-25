#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include "framework/core/log.h"
#include "framework/widgets/panel.h"
#include "framework/widgets/label.h"

namespace orf {

static void glfwErrorCallback(int error, const char* description) {
    LOG_ERROR("[GLFW Error " << error << "] " << description);
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->renderer().setViewportSize(width, height);
        // We'll also call the member function for any additional logic
    }
    glViewport(0, 0, width, height);
}

bool Application::init(int width, int height, const char* title) {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        LOG_ERROR("Fatal: glfwInit() failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        LOG_ERROR("Fatal: glfwCreateWindow() failed");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Fatal: gladLoadGLLoader() failed");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    LOG_INFO("OpenGL Vendor:   " << glGetString(GL_VENDOR));
    LOG_INFO("OpenGL Renderer: " << glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version:  " << glGetString(GL_VERSION));

    glfwGetFramebufferSize(m_window, &m_fbWidth, &m_fbHeight);
    if (!m_renderer.init(m_fbWidth, m_fbHeight)) {
        LOG_ERROR("Fatal: Renderer2D initialization failed");
        shutdown();
        return false;
    }

    // TODO: Implement proper cross-platform font discovery/management.
    // For now, we use hardcoded paths for common system fonts.
#ifdef __APPLE__
    const char* defaultFontPath = "/System/Library/Fonts/Geneva.ttf";
#elif defined(_WIN32)
    const char* defaultFontPath = "C:\\Windows\\Fonts\\arial.ttf";
#else
    const char* defaultFontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif

    if (!m_font.load(defaultFontPath, 24)) {
        LOG_ERROR("Fatal: Failed to load default font at " << defaultFontPath);
        shutdown();
        return false;
    }

    m_rootWidget = std::make_unique<orf::RootWidget>(m_font);
    m_rootWidget->resize(m_fbWidth, m_fbHeight);

    // Build a small test scene: Root -> MainPanel -> Label + SidePanel -> SidebarLabel.
    auto mainPanel = std::make_unique<Panel>(Color{0.18f, 0.18f, 0.18f, 1.0f});
    mainPanel->setBounds({50, 50, 600, 400});

    auto mainLabel = std::make_unique<Label>("Main Panel", Color::white());
    mainLabel->setBounds({60, 60, 200, 30});
    mainPanel->addChild(std::move(mainLabel));

    auto sidePanel = std::make_unique<Panel>(Color{0.25f, 0.25f, 0.25f, 1.0f});
    sidePanel->setBounds({400, 60, 180, 320});

    auto sideLabel = std::make_unique<Label>("Sidebar", Color{0.8f, 0.8f, 0.8f, 1.0f});
    sideLabel->setBounds({410, 70, 100, 30});
    sidePanel->addChild(std::move(sideLabel));

    mainPanel->addChild(std::move(sidePanel));
    m_rootWidget->addChild(std::move(mainPanel));

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) app->onFramebufferResize(w, h);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) app->onKey(key, action);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) app->onMouseMove(xpos, ypos);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) app->onMouseButton(button, action);
    });

    glfwSwapInterval(1);
    glViewport(0, 0, m_fbWidth, m_fbHeight);

    return true;
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_renderer.begin();
        m_rootWidget->paintIfDirty(m_renderer);
        m_renderer.end();

        glfwSwapBuffers(m_window);
    }
}

void Application::shutdown() {
    m_renderer.shutdown();
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Application::onFramebufferResize(int w, int h) {
    m_fbWidth = w;
    m_fbHeight = h;
    m_renderer.setViewportSize(w, h);
    glViewport(0, 0, w, h);
    if (m_rootWidget) m_rootWidget->resize(w, h);
}

void Application::onKey(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
    if (m_rootWidget) m_rootWidget->dispatchKey(key, action);
}

void Application::onMouseMove(double x, double y) {
    if (m_rootWidget) m_rootWidget->dispatchMouseMove((float)x, (float)y);
}

void Application::onMouseButton(int button, int action) {
    if (m_rootWidget) {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        if (action == GLFW_PRESS)
            m_rootWidget->dispatchMousePress((float)x, (float)y, button);
        else
            m_rootWidget->dispatchMouseRelease((float)x, (float)y, button);
    }
}

} // namespace orf
