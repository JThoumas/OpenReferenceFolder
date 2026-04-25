#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include "framework/core/log.h"

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
        return false;
    }

    // Load font
    if (!m_font.load("/System/Library/Fonts/Geneva.ttf", 24)) {
        LOG_ERROR("Warning: Failed to load Geneva.ttf");
    }

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
        
        // Reproduce Phase 2 test render from main.cpp
        m_renderer.drawRect({0, 0, (float)m_fbWidth, (float)m_fbHeight}, {0.13f, 0.13f, 0.13f, 1.0f});
        m_renderer.drawRect({100, 100, 300, 200}, {0.3f, 0.5f, 0.8f, 1.0f});
        m_renderer.drawRect({250, 200, 200, 150}, {0.8f, 0.3f, 0.3f, 0.9f});

        if (m_font.atlasTextureID() != 0) {
            m_renderer.drawText("OpenReferenceFolder", 60, 90, m_font, orf::Color::white());
            m_renderer.drawText("Batched font rendering is working!", 60, 130, m_font, {0.8f, 0.8f, 0.8f, 1.0f});
        }

        m_renderer.pushClip({500, 100, 200, 100});
        m_renderer.drawRect({450, 50, 300, 200}, {0.2f, 0.7f, 0.3f, 0.8f});
        if (m_font.atlasTextureID() != 0) {
            m_renderer.drawText("This text should be clipped.", 460, 150, m_font, orf::Color::black());
        }
        m_renderer.popClip();

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
}

void Application::onKey(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Application::onMouseMove(double x, double y) {
    // For now, nothing
}

void Application::onMouseButton(int button, int action) {
    // For now, nothing
}

} // namespace orf
