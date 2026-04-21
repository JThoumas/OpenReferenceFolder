#pragma once
#include "shader.h"
#include "font.h"
#include <glad/glad.h>
#include <cstdint>
#include <array>

namespace orf {

struct Rect {
    float x, y, width, height;
};

struct Color {
    float r, g, b, a;
    static constexpr Color white()       { return {1.0f, 1.0f, 1.0f, 1.0f}; }
    static constexpr Color black()       { return {0.0f, 0.0f, 0.0f, 1.0f}; }
    static constexpr Color transparent() { return {0.0f, 0.0f, 0.0f, 0.0f}; }
};

class Renderer2D {
public:
    static constexpr uint32_t MAX_QUADS    = 10000;
    static constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
    static constexpr uint32_t MAX_INDICES  = MAX_QUADS * 6;
    static constexpr uint32_t MAX_TEXTURES = 8;

    Renderer2D()  = default;
    ~Renderer2D() = default;

    Renderer2D(const Renderer2D&)            = delete;
    Renderer2D& operator=(const Renderer2D&) = delete;

    // Call once after OpenGL context is current
    bool init(int viewportWidth, int viewportHeight);
    void shutdown();

    // Call when window is resized
    void setViewportSize(int width, int height);

    // Frame lifecycle — call begin() before any draw calls, end() after
    void begin();
    void end();     // flushes remaining batch to GPU

    // Draw primitives
    void drawRect(const Rect& rect, const Color& color);
    void drawTexturedRect(const Rect& rect, GLuint textureID, const Color& tint = Color::white());
    void drawText(const std::string& text, float x, float y, const Font& font, const Color& color);

    void pushClip(const Rect& rect);
    void popClip();

private:
    struct Vertex {
        float x, y;       // position
        float u, v;       // UV
        float r, g, b, a; // color
        float texIndex;   // -1 = flat color
    };

    void flush();         // upload batch + draw call
    int  getTexSlot(GLuint textureID); // returns slot index, flushes if full

    Shader   m_shader;
    GLuint   m_vao = 0;
    GLuint   m_vbo = 0;
    GLuint   m_ibo = 0;  // index buffer (shared across all quads)

    std::array<Vertex, MAX_VERTICES> m_vertexBuffer;
    uint32_t m_quadCount = 0;

    std::array<GLuint, MAX_TEXTURES> m_textureSlots{};
    uint32_t m_textureSlotCount = 0;

    // 1x1 white texture — used as the "no texture" fallback
    GLuint m_whiteTexture = 0;

    float m_projection[16]{};  // orthographic matrix
    int   m_vpWidth  = 0;
    int   m_vpHeight = 0;
};

} // namespace orf
