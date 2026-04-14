#include "renderer2d.h"
#include "framework/core/log.h"
#include <cstring>
#include <cstddef>

namespace orf {

// --- Shader sources ---
static constexpr const char* VERT_SRC = R"(
#version 330 core
layout(location = 0) in vec2  a_Position;
layout(location = 1) in vec2  a_UV;
layout(location = 2) in vec4  a_Color;
layout(location = 3) in float a_TexIndex;

out vec2  v_UV;
out vec4  v_Color;
out float v_TexIndex;

uniform mat4 u_Projection;

void main() {
    v_UV        = a_UV;
    v_Color     = a_Color;
    v_TexIndex  = a_TexIndex;
    gl_Position = u_Projection * vec4(a_Position, 0.0, 1.0);
}
)";

static constexpr const char* FRAG_SRC = R"(
#version 330 core
in vec2  v_UV;
in vec4  v_Color;
in float v_TexIndex;

out vec4 fragColor;

uniform sampler2D u_Textures[8];

void main() {
    if (v_TexIndex < 0.0) {
        fragColor = v_Color;
    } else {
        int idx = int(v_TexIndex);
        fragColor = texture(u_Textures[idx], v_UV) * v_Color;
    }
}
)";

// Builds a column-major orthographic projection matrix.
// Maps (0,0) top-left to (w,h) bottom-right in clip space.
static void buildOrtho(float* out, float w, float h) {
    memset(out, 0, 16 * sizeof(float));
    out[0]  =  2.0f / w;
    out[5]  = -2.0f / h;   // Flipped: Y=0 is top of screen
    out[10] = -1.0f;
    out[12] = -1.0f;
    out[13] =  1.0f;
    out[15] =  1.0f;
}

bool Renderer2D::init(int vpWidth, int vpHeight) {
    // Compile shaders
    if (!m_shader.load(VERT_SRC, FRAG_SRC)) {
        LOG_ERROR("Renderer2D: shader compilation failed");
        return false;
    }

    // Pre-build the index buffer — same pattern for every quad:
    // two triangles (0,1,2) and (2,3,0) per quad
    std::array<uint32_t, MAX_INDICES> indices{};
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MAX_INDICES; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    // VAO — records how vertex data is laid out
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // VBO — dynamic, updated every frame
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * MAX_VERTICES,
                 nullptr,
                 GL_DYNAMIC_DRAW);

    // IBO — static, never changes
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint32_t) * MAX_INDICES,
                 indices.data(),
                 GL_STATIC_DRAW);

    // Vertex attribute layout — must match the Vertex struct exactly
    // location 0: position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, x));
    // location 1: UV
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, u));
    // location 2: color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, r));
    // location 3: texture index
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, texIndex));

    glBindVertexArray(0);

    // 1x1 white texture used when no texture is needed but the
    // shader always samples — avoids branching in hot path
    uint32_t white = 0xFFFFFFFF;
    glGenTextures(1, &m_whiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_whiteTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, &white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload sampler indices once — they don't change
    m_shader.bind();
    int samplers[MAX_TEXTURES] = {0,1,2,3,4,5,6,7};
    glUniform1iv(
        glGetUniformLocation(m_shader.programID(), "u_Textures"),
        MAX_TEXTURES,
        samplers
    );
    m_shader.unbind();

    setViewportSize(vpWidth, vpHeight);

    // Enable alpha blending — required for text and transparent UI elements
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LOG_INFO("Renderer2D initialized (" << vpWidth << "x" << vpHeight << ")");
    return true;
}

void Renderer2D::shutdown() {
    if (m_vao)          glDeleteVertexArrays(1, &m_vao);
    if (m_vbo)          glDeleteBuffers(1, &m_vbo);
    if (m_ibo)          glDeleteBuffers(1, &m_ibo);
    if (m_whiteTexture) glDeleteTextures(1, &m_whiteTexture);
    m_vao = m_vbo = m_ibo = m_whiteTexture = 0;
}

void Renderer2D::setViewportSize(int w, int h) {
    m_vpWidth  = w;
    m_vpHeight = h;
    buildOrtho(m_projection, (float)w, (float)h);
}

void Renderer2D::begin() {
    m_quadCount        = 0;
    m_textureSlotCount = 0;
}

void Renderer2D::end() {
    flush();
}

void Renderer2D::flush() {
    if (m_quadCount == 0) return;

    // Bind all active texture slots
    for (uint32_t i = 0; i < m_textureSlotCount; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureSlots[i]);
    }

    // Upload only the portion of the buffer we actually used
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(Vertex) * m_quadCount * 4,
                    m_vertexBuffer.data());

    m_shader.bind();
    m_shader.setMat4("u_Projection", m_projection);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(m_quadCount * 6),
                   GL_UNSIGNED_INT,
                   nullptr);

    // Reset for next batch
    m_quadCount        = 0;
    m_textureSlotCount = 0;
}

int Renderer2D::getTexSlot(GLuint textureID) {
    // Check if already bound this batch
    for (uint32_t i = 0; i < m_textureSlotCount; ++i)
        if (m_textureSlots[i] == textureID) return static_cast<int>(i);

    // Flush if we've hit the texture limit
    if (m_textureSlotCount >= MAX_TEXTURES) flush();

    m_textureSlots[m_textureSlotCount] = textureID;
    return static_cast<int>(m_textureSlotCount++);
}

void Renderer2D::drawRect(const Rect& r, const Color& c) {
    if (m_quadCount >= MAX_QUADS) flush();

    uint32_t base = m_quadCount * 4;
    // top-left
    m_vertexBuffer[base+0] = {r.x,           r.y,            0,0, c.r,c.g,c.b,c.a, -1.0f};
    // top-right
    m_vertexBuffer[base+1] = {r.x + r.width, r.y,            1,0, c.r,c.g,c.b,c.a, -1.0f};
    // bottom-right
    m_vertexBuffer[base+2] = {r.x + r.width, r.y + r.height, 1,1, c.r,c.g,c.b,c.a, -1.0f};
    // bottom-left
    m_vertexBuffer[base+3] = {r.x,           r.y + r.height, 0,1, c.r,c.g,c.b,c.a, -1.0f};
    ++m_quadCount;
}

void Renderer2D::drawTexturedRect(const Rect& r, GLuint texID, const Color& tint) {
    if (m_quadCount >= MAX_QUADS) flush();

    float slot = static_cast<float>(getTexSlot(texID));
    uint32_t base = m_quadCount * 4;
    m_vertexBuffer[base+0] = {r.x,           r.y,            0,0, tint.r,tint.g,tint.b,tint.a, slot};
    m_vertexBuffer[base+1] = {r.x + r.width, r.y,            1,0, tint.r,tint.g,tint.b,tint.a, slot};
    m_vertexBuffer[base+2] = {r.x + r.width, r.y + r.height, 1,1, tint.r,tint.g,tint.b,tint.a, slot};
    m_vertexBuffer[base+3] = {r.x,           r.y + r.height, 0,1, tint.r,tint.g,tint.b,tint.a, slot};
    ++m_quadCount;
}

} // namespace orf
