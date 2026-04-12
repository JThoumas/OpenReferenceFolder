#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Callback function to adjust the viewport when the window is resized
static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback function to print GLFW errors to the console
static void glfwErrorCallback(int error, const char* description) {
    std::cerr << "[GLFW Error " << error << "] " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        std::cerr << "Fatal: glfwInit() failed" << std::endl;
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
        std::cerr << "Fatal: glfwCreateWindow() failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load all OpenGL 3.3 function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Fatal: gladLoadGLLoader() failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Set the viewport and register the framebuffer size callback
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
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

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}