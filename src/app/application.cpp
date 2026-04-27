#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include "framework/core/log.h"
#include "framework/widgets/panel.h"
#include "app/widgets/file_browser_widget.h"
#include "app/widgets/display_center_widget.h"
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

    if (!m_database.open("orf_data.db")) {
        LOG_ERROR("Fatal: could not open database");
        return false;
    }

    m_thumbs.init();

    // Theme initialization
    std::filesystem::create_directories("themes");
    if (!std::filesystem::exists("themes/orf-dark.json")) {
        ThemeManager::get().resetToDefault();
        ThemeManager::get().saveToFile("themes/orf-dark.json");
    }
    ThemeManager::get().loadFromFile("themes/orf-dark.json");

    ThemeManager::get().onThemeChanged([this]() {
        markAllDirty();
    });

    if (!m_font.load("assets/fonts/Inter-Regular.ttf", 16)) {
        LOG_WARN("Failed to load default font, text rendering may fail");
    }

    m_cursorArrow  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_cursorHSplit = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_cursorVSplit = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    m_dockManager = std::make_unique<DockManager>(m_font);

    loadLayout();
    m_dockManager->resize(m_fbWidth, m_fbHeight);

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

        m_thumbs.uploadPending();

        if (m_dockManager) m_dockManager->updateFloatingWindows();

        glClearColor(theme().windowBackground.r, theme().windowBackground.g, theme().windowBackground.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_renderer.begin();
        if (m_dockManager) m_dockManager->paint(m_renderer);
        m_renderer.end();

        if (m_dockManager) m_dockManager->paintFloatingWindows();

        glfwSwapBuffers(m_window);
    }
}

void Application::shutdown() {
    saveLayout();

    m_thumbs.shutdown();
    m_database.close();

    m_renderer.shutdown();

    if (m_cursorArrow)  glfwDestroyCursor(m_cursorArrow);
    if (m_cursorHSplit) glfwDestroyCursor(m_cursorHSplit);
    if (m_cursorVSplit) glfwDestroyCursor(m_cursorVSplit);

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
    if (m_dockManager) m_dockManager->resize(w, h);
}

void Application::onKey(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Application::onMouseMove(double x, double y) {
    if (m_dockManager) {
        m_dockManager->onMouseMove((float)x, (float)y);
        updateCursor((float)x, (float)y);
    }
}

void Application::onMouseButton(int button, int action) {
    if (m_dockManager) {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        if (action == GLFW_PRESS)
            m_dockManager->onMousePress((float)x, (float)y, button);
        else
            m_dockManager->onMouseRelease((float)x, (float)y, button);
    }
}

void Application::onScroll(double yOffset) {
}

void Application::markAllDirty() {
    if (m_dockManager && m_dockManager->root()) {
        m_dockManager->root()->markDirty();
    }
}

void Application::saveLayout(const std::string& name) {
    if (!m_dockManager) return;
    
    nlohmann::json j = m_dockManager->serialize();
    std::string path = (name == "layout") ? "layout.json" : "workspaces/" + name + ".json";
    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
        LOG_INFO("Layout saved to " << path);
    }
}

void Application::loadLayout(const std::string& name) {
    std::string path = (name == "layout") ? "layout.json" : "workspaces/" + name + ".json";
    std::ifstream file(path);
    if (file.is_open()) {
        try {
            nlohmann::json j;
            file >> j;
            auto root = DockNode::deserialize(j, m_font, &m_database, &m_thumbs);
            if (root) {
                m_dockManager->setRoot(std::move(root));
                LOG_INFO("Layout loaded from " << path);
                return;
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to load layout: " << e.what());
        }
    }

    if (name != "layout") return; // Don't build fallback for custom workspace if file missing

    // Fallback: Build the Real Dock Layout
    auto browser = std::make_unique<FileBrowserWidget>(m_database, m_thumbs, m_font);
    browser->refresh();
    browser->setViewMode(BrowserViewMode::Grid);

    auto browserPanel = std::make_unique<PanelNode>("Reference Folder", std::move(browser), m_font);

    auto dcWidgetPtr = std::make_unique<DisplayCenterWidget>(m_font, ThemeManager::get(), m_database);
    DisplayCenterWidget* dcWidget = dcWidgetPtr.get();
    
    dcWidget->setOnSaveWorkspace([this](const std::string& name) {
        saveLayout(name);
    });
    dcWidget->setOnApplyWorkspace([this](const std::string& name) {
        loadLayout(name);
    });
    dcWidget->setOnDeleteWorkspace([](const std::string& name) {
        // file deletion handled in dcWidget for now
    });

    auto dcPanel = std::make_unique<PanelNode>("Display Center", std::move(dcWidgetPtr), m_font);

    auto root = std::make_unique<SplitNode>(
        SplitAxis::Horizontal,
        std::move(browserPanel),
        std::move(dcPanel),
        0.30f
    );

    m_dockManager->setRoot(std::move(root));
}

void Application::updateCursor(float x, float y) {
    auto axis = m_dockManager->hitTestDivider(x, y);
    if (!axis.has_value())
        glfwSetCursor(m_window, m_cursorArrow);
    else if (*axis == SplitAxis::Horizontal)
        glfwSetCursor(m_window, m_cursorHSplit);
    else
        glfwSetCursor(m_window, m_cursorVSplit);
}

} // namespace orf
