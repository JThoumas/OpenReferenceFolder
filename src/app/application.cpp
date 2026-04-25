#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include "framework/core/log.h"
#include "framework/widgets/panel.h"
#include "framework/widgets/label.h"
#include "framework/widgets/button.h"
#include "framework/widgets/scroll_view.h"

namespace orf {

static void glfwErrorCallback(int error, const char* description) {
    LOG_ERROR("[GLFW Error " << error << "] " << description);
}

bool Application::init(int width, int height, const char* title) {
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return false;
    }

    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        return false;
    }

    glfwGetFramebufferSize(m_window, &m_fbWidth, &m_fbHeight);

    if (!m_renderer.init(m_fbWidth, m_fbHeight)) {
        LOG_ERROR("Failed to initialize Renderer2D");
        return false;
    }

    if (!m_font.load("assets/fonts/Inter-Regular.ttf", 16)) {
        LOG_WARN("Failed to load default font, text rendering may fail");
    }

    m_rootWidget = std::make_unique<RootWidget>(m_font);
    m_rootWidget->resize(m_fbWidth, m_fbHeight);

    // Test Scene
    auto mainPanel = std::make_unique<Panel>(Color{0.18f, 0.18f, 0.2f, 1.0f});
    mainPanel->setBounds({50, 50, 600, 400});

    auto titleLabel = std::make_unique<Label>("OpenReferenceFolder — Widget System");
    titleLabel->setBounds({60, 60, 580, 30});
    mainPanel->addChild(std::move(titleLabel));

    auto sidePanel = std::make_unique<Panel>(Color{0.12f, 0.12f, 0.15f, 1.0f});
    sidePanel->setBounds({400, 60, 180, 320});

    auto sideLabel = std::make_unique<Label>("Sidebar", Color{0.8f, 0.8f, 0.8f, 1.0f});
    sideLabel->setBounds({410, 70, 100, 30});
    sidePanel->addChild(std::move(sideLabel));
    mainPanel->addChild(std::move(sidePanel));

    auto btn = std::make_unique<Button>("Click Me", m_font);
    btn->setBounds({170, 50, 120, 36});
    btn->setOnClick([]() {
        LOG_INFO("Button clicked!");
    });
    mainPanel->addChild(std::move(btn));

    auto scroll = std::make_unique<ScrollView>();
    scroll->setBounds({10, 100, 380, 280});
    scroll->setContentHeight(1000.0f);
    for (int i = 0; i < 20; ++i) {
        auto item = std::make_unique<Label>("Scroll Item #" + std::to_string(i));
        item->setBounds({10, 10.0f + i * 40.0f, 300, 30});
        scroll->addChild(std::move(item));
    }
    mainPanel->addChild(std::move(scroll));

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

void Application::onScroll(double yOffset) {
    if (m_rootWidget) m_rootWidget->dispatchScroll((float)yOffset);
}

} // namespace orf
