#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <framework/core/log.h>
#include <framework/renderer/renderer2d.h>
#include <framework/renderer/font.h>

// Callback function to adjust the viewport when the window is resized
static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    // Update the renderer's viewport size
    auto* renderer = static_cast<orf::Renderer2D*>(glfwGetWindowUserPointer(window));
    if (renderer) {
        renderer->setViewportSize(width, height);
    }
}

// Callback function to print GLFW errors to the console
static void glfwErrorCallback(int error, const char* description) {
    LOG_ERROR("[GLFW Error " << error << "] " << description);
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        LOG_ERROR("Fatal: glfwInit() failed");
        return -1;
    }

    // Request an OpenGL 3.3 core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(
        1280, 720,
        "Open Reference Folder",
        nullptr, nullptr // No fullscreen, no shared context
    );

    // Check if window creation succeeded
    if (!window) {
        LOG_ERROR("Fatal: glfwCreateWindow() failed");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load all OpenGL 3.3 function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Fatal: gladLoadGLLoader() failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Print OpenGL diagnostic info
    LOG_INFO("OpenGL Vendor:   " << glGetString(GL_VENDOR));
    LOG_INFO("OpenGL Renderer: " << glGetString(GL_RENDERER));
    LOG_INFO("OpenGL Version:  " << glGetString(GL_VERSION));
    LOG_INFO("GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize Renderer2D
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    orf::Renderer2D renderer;
    if (!renderer.init(fbWidth, fbHeight)) {
        LOG_ERROR("Fatal: Renderer2D initialization failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Load a font for testing
    orf::Font font;
    bool fontLoaded = font.load("/System/Library/Fonts/Geneva.ttf", 24);
    if (!fontLoaded) {
        LOG_ERROR("Warning: Failed to load Geneva.ttf, text rendering will be skipped.");
    }

    // Store the renderer in the window's user pointer for callbacks
    glfwSetWindowUserPointer(window, &renderer);

    // Press Escape to close the window
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GLFW_TRUE);
        }
    });

    // Set the viewport and register the framebuffer size callback
    glViewport(0, 0, fbWidth, fbHeight);

    // Register the framebuffer size callback to adjust the viewport on window resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Enable vsync
    glfwSwapInterval(1);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.13f, 0.13f, 0.13f, 1.0f); // Clear to a dark gray color
        glClear(GL_COLOR_BUFFER_BIT);

        // Render with Renderer2D
        renderer.begin();
        
        // Background
        renderer.drawRect({0, 0, (float)fbWidth, (float)fbHeight}, {0.13f, 0.13f, 0.13f, 1.0f});

        // Test Shapes
        renderer.drawRect({100, 100, 300, 200}, {0.3f, 0.5f, 0.8f, 1.0f});
        renderer.drawRect({250, 200, 200, 150}, {0.8f, 0.3f, 0.3f, 0.9f});

        // Test Text
        if (fontLoaded) {
            renderer.drawText("OpenReferenceFolder", 60, 90, font, orf::Color::white());
            renderer.drawText("Batched font rendering is working!", 60, 130, font, {0.8f, 0.8f, 0.8f, 1.0f});
        }

        // Test Clipping
        renderer.pushClip({500, 100, 200, 100});
        renderer.drawRect({450, 50, 300, 200}, {0.2f, 0.7f, 0.3f, 0.8f}); // Partially clipped
        if (fontLoaded) {
            renderer.drawText("This text should be clipped.", 460, 150, font, orf::Color::black());
        }
        renderer.popClip();

        renderer.end();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    renderer.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}