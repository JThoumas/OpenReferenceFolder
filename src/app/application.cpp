#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include "framework/core/log.h"
#include "framework/widgets/panel.h"
#include "app/widgets/file_browser_widget.h"
#include "app/widgets/display_center_widget.h"
#include "app/widgets/pure_view_widget.h"
#include "framework/regions/collapsible_region.h"
#include "framework/regions/center_work_area.h"
#include "framework/regions/top_bar.h"
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
        LOG_WARN("Failed to load default font, trying system fallbacks...");
        if (!m_font.load("/Library/Fonts/Arial.ttf", 16)) {
            if (!m_font.load("/System/Library/Fonts/Helvetica.ttc", 16)) {
                if (!m_font.load("/Library/Fonts/Arial Unicode.ttf", 16)) {
                    LOG_ERROR("All fallback fonts failed. Text will not render.");
                }
            }
        }
    }

    m_cursorArrow  = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_cursorHSplit = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_cursorVSplit = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

    m_regionManager = std::make_unique<RegionManager>(m_font);
    m_regionManager->init(m_fbWidth, m_fbHeight);

    // ── Left Region: File Browser ──────────────────────────────────────────
    auto browser = std::make_unique<FileBrowserWidget>(m_database, m_thumbs, m_font);
    browser->refresh();
    browser->setViewMode(BrowserViewMode::Grid);

    // Single-click → update sidebar PureView
    browser->setOnSelect([this](const ORFNode& node) {
        if (!node.hasRealPath() || node.isFolder()) return;
        if (m_sidebarPureView)
            m_sidebarPureView->loadFile(node.realPath);
    });

    // Double-click → pin a PureView tab in the Center
    browser->setOnOpen([this](const ORFNode& node) {
        if (!node.hasRealPath() || node.isFolder()) return;
        std::string tabTitle = node.displayName;

        // Reuse existing tab if already open
        if (!m_regionManager->center()->focusTab(tabTitle)) {
            auto pv = std::make_unique<PureViewWidget>(
                PureViewWidget::Mode::Pinned, m_thumbs, m_font);
            pv->loadFile(node.realPath);
            m_regionManager->center()->addTab(tabTitle, std::move(pv));
        }
    });

    m_regionManager->leftRegion()->addTab("Files", std::move(browser));

    // ── Right Region: Sidebar PureView + Display Center ───────────────────
    auto sidePreview = std::make_unique<PureViewWidget>(
        PureViewWidget::Mode::Sidebar, m_thumbs, m_font);
    m_sidebarPureView = sidePreview.get(); // store raw ptr for callbacks above
    m_regionManager->rightRegion()->addTab("Preview", std::move(sidePreview));

    auto dcWidgetPtr = std::make_unique<DisplayCenterWidget>(m_font, ThemeManager::get(), m_database);
    dcWidgetPtr->setOnSaveWorkspace([this](const std::string& name) {
        saveLayout(name);
    });
    dcWidgetPtr->setOnApplyWorkspace([this](const std::string& name) {
        loadLayout(name);
    });
    
    m_regionManager->rightRegion()->addTab("Display Center", std::move(dcWidgetPtr));

    // ── Bottom Region: placeholder for future metadata / log panel ─────────
    auto logPanel = std::make_unique<Panel>(theme().panelBackground);
    m_regionManager->bottomRegion()->addTab("Log", std::move(logPanel));
    m_regionManager->bottomRegion()->setCollapsed(true);

    // ── Center: Start with a welcome / empty state ─────────────────────────
    auto welcome = std::make_unique<Panel>(theme().windowBackground);
    m_regionManager->center()->addTab("Welcome", std::move(welcome));

    // ── Top Bar Menus ──────────────────────────────────────────────────────
    Menu fileMenu;
    fileMenu.label = "File";
    fileMenu.entries = {
        {"Add Files...",  [this]() { /* open file dialog */ }},
        {"Add Folder...", [this]() { /* open folder dialog */ }},
        {"", {}, true}, // separator
        {"Save Workspace", [this]() { saveLayout(); }},
        {"", {}, true},
        {"Quit", [this]() { glfwSetWindowShouldClose(m_window, GLFW_TRUE); }}
    };

    Menu viewMenu;
    viewMenu.label = "View";
    viewMenu.entries = {
        {"Toggle Left Sidebar",   [this]() { m_regionManager->leftRegion()->toggleCollapsed(); }},
        {"Toggle Right Sidebar",  [this]() { m_regionManager->rightRegion()->toggleCollapsed(); }},
        {"Toggle Bottom Panel",   [this]() { m_regionManager->bottomRegion()->toggleCollapsed(); }},
        {"", {}, true},
        {"Welcome Screen",        [this]() { m_regionManager->center()->focusTab("Welcome"); }}
    };

    Menu helpMenu;
    helpMenu.label = "Help";
    helpMenu.entries = {
        {"About ORF", []() { LOG_INFO("OpenReferenceFolder v0.7"); }}
    };

    m_regionManager->topBar()->addMenu(std::move(fileMenu));
    m_regionManager->topBar()->addMenu(std::move(viewMenu));
    m_regionManager->topBar()->addMenu(std::move(helpMenu));

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

        const Theme& t = theme();
        glClearColor(t.windowBackground.r, t.windowBackground.g, t.windowBackground.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_renderer.begin();
        if (m_regionManager) m_regionManager->paint(m_renderer);
        m_renderer.end();

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
    if (m_regionManager) m_regionManager->resize(w, h);
}

void Application::onKey(int key, int action) {
    if (m_regionManager) m_regionManager->onKey(key, action);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Application::onMouseMove(double x, double y) {
    if (m_regionManager) {
        m_regionManager->onMouseMove((float)x, (float)y);
        updateCursor((float)x, (float)y);
    }
}

void Application::onMouseButton(int button, int action) {
    if (m_regionManager) {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        if (action == GLFW_PRESS)
            m_regionManager->onMousePress((float)x, (float)y, button);
        else
            m_regionManager->onMouseRelease((float)x, (float)y, button);
    }
}

void Application::onScroll(double yOffset) {
    if (m_regionManager) m_regionManager->onScroll((float)yOffset);
}

void Application::markAllDirty() {
    // Phase 7 layout is painted unconditionally for now.
    // In Phase 8 we'll add a more surgical dirty-flag system for regions.
}

void Application::saveLayout(const std::string& name) {
    if (!m_regionManager) return;
    
    nlohmann::json j;
    m_regionManager->serializeLayout(j);
    std::string path = (name == "layout") ? "layout.json" : "workspaces/" + name + ".json";
    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
        LOG_INFO("Layout saved to " << path);
    }
}

void Application::loadLayout(const std::string& name) {
    if (!m_regionManager) return;

    std::string path = (name == "layout") ? "layout.json" : "workspaces/" + name + ".json";
    std::ifstream file(path);
    if (file.is_open()) {
        try {
            nlohmann::json j;
            file >> j;
            m_regionManager->deserializeLayout(j);
            LOG_INFO("Layout loaded from " << path);
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to load layout: " << e.what());
        }
    }
}

void Application::updateCursor(float x, float y) {
    // RegionManager handles resize grips
    // For now, reset to arrow. Grip-specific cursors will be added in Phase 8.
    glfwSetCursor(m_window, m_cursorArrow);
}

} // namespace orf
