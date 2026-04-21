#pragma once
#include <glad/glad.h>
#include <string>
#include <array>

namespace orf {

struct GlyphMetrics {
    float u0, v0, u1, v1;   // UV coords in the atlas texture
    int   width, height;     // glyph bitmap size in pixels
    int   bearingX;          // offset from cursor to left of glyph
    int   bearingY;          // offset from baseline to top of glyph
    int   advance;           // how far to move cursor after this glyph
};

class Font {
public:
    Font()  = default;
    ~Font();

    Font(const Font&)            = delete;
    Font& operator=(const Font&) = delete;

    // Load a .ttf font file at a given pixel height.
    // Builds the glyph atlas GPU texture automatically.
    bool load(const std::string& ttfPath, int pixelHeight);

    GLuint              atlasTextureID() const { return m_atlasID; }
    int                 atlasWidth()     const { return m_atlasW; }
    int                 atlasHeight()    const { return m_atlasH; }
    int                 lineHeight()     const { return m_lineHeight; }
    const GlyphMetrics& glyph(char c)   const { return m_glyphs[static_cast<unsigned char>(c)]; }

private:
    GLuint                        m_atlasID    = 0;
    int                           m_atlasW     = 0;
    int                           m_atlasH     = 0;
    int                           m_lineHeight = 0;
    std::array<GlyphMetrics, 128> m_glyphs{};
};

} // namespace orf
