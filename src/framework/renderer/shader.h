#pragma once
#include <glad/glad.h>
#include <string>
#include <string_view>

namespace orf {

class Shader {
public:
    Shader() = default;
    ~Shader();

    // Non-copyable — OpenGL handles shouldn't be duplicated
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    // Movable
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // Compile and link from GLSL source strings.
    // Returns false and logs the error if compilation fails.
    bool load(std::string_view vertSrc, std::string_view fragSrc);

    void bind()   const;
    void unbind() const;

    // Uniform setters — expand as needed
    void setInt  (std::string_view name, int value)         const;
    void setFloat(std::string_view name, float value)       const;
    void setVec2 (std::string_view name, float x, float y) const;
    void setMat4 (std::string_view name, const float* mat)  const;

    bool isValid() const { return m_programID != 0; }

    GLuint programID() const { return m_programID; }

private:
    GLuint m_programID = 0;

    static GLuint compileStage(GLenum type, std::string_view src);
};

} // namespace orf
