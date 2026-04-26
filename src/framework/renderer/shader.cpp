#include "shader.h"
#include "framework/core/log.h"
#include <vector>

namespace orf {

Shader::~Shader() {
    if (m_programID) glDeleteProgram(m_programID);
}

Shader::Shader(Shader&& other) noexcept
    : m_programID(other.m_programID) {
    other.m_programID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (m_programID) glDeleteProgram(m_programID);
        m_programID = other.m_programID;
        other.m_programID = 0;
    }
    return *this;
}

GLuint Shader::compileStage(GLenum type, std::string_view src) {
    GLuint id = glCreateShader(type);
    const char* ptr = src.data();
    GLint len       = static_cast<GLint>(src.size());
    glShaderSource(id, 1, &ptr, &len);
    glCompileShader(id);

    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetShaderInfoLog(id, logLen, nullptr, log.data());
        LOG_ERROR("Shader compile error:\n" << log.data());
        glDeleteShader(id);
        return 0;
    }
    return id;
}

bool Shader::load(std::string_view vertSrc, std::string_view fragSrc) {
    GLuint vert = compileStage(GL_VERTEX_SHADER,   vertSrc);
    GLuint frag = compileStage(GL_FRAGMENT_SHADER, fragSrc);

    if (!vert || !frag) {
        if (vert) glDeleteShader(vert);
        if (frag) glDeleteShader(frag);
        return false;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vert);
    glAttachShader(m_programID, frag);
    glLinkProgram(m_programID);

    // Shaders are copied into the program — safe to delete now
    glDeleteShader(vert);
    glDeleteShader(frag);

    GLint success = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetProgramInfoLog(m_programID, logLen, nullptr, log.data());
        LOG_ERROR("Shader link error:\n" << log.data());
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }

    return true;
}

void Shader::bind()   const { glUseProgram(m_programID); }
void Shader::unbind() const { glUseProgram(0); }

void Shader::setInt(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.data()), value);
}

void Shader::setFloat(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(m_programID, name.data()), value);
}

void Shader::setVec2(std::string_view name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_programID, name.data()), x, y);
}

void Shader::setMat4(std::string_view name, const float* mat) const {
    glUniformMatrix4fv(
        glGetUniformLocation(m_programID, name.data()),
        1, GL_FALSE, mat
    );
}

} // namespace orf
