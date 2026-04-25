#include "font.h"
#include "framework/core/log.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <cstring>
#include <algorithm>

namespace orf {

static int nextPOT(int n) {
    int v = 1;
    while (v < n) v <<= 1;
    return v;
}

Font::~Font() {
    if (m_atlasID) glDeleteTextures(1, &m_atlasID);
}

bool Font::load(const std::string& path, int pixelHeight) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        LOG_ERROR("Font: FreeType init failed");
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        LOG_ERROR("Font: failed to load " << path);
        FT_Done_FreeType(ft);
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelHeight);
    m_lineHeight = pixelHeight;

    // Use NO_BITMAP to prevent loading embedded monochrome strikes (which break our assumptions about 1 byte = 1 pixel)
    const int loadFlags = FT_LOAD_RENDER | FT_LOAD_NO_BITMAP;

    // First pass: measure total atlas dimensions
    int totalWidth = 0, maxHeight = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, loadFlags)) continue;
        totalWidth += face->glyph->bitmap.width + 2;
        maxHeight   = std::max(maxHeight, (int)face->glyph->bitmap.rows);
    }

    // Use Power-of-Two dimensions for maximum compatibility
    m_atlasW = nextPOT(totalWidth);
    m_atlasH = nextPOT(maxHeight);

    if (m_atlasW == 0 || m_atlasH == 0) {
        LOG_ERROR("Font: zero atlas dimensions for " << path);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return false;
    }

    // Allocate atlas CPU buffer (RGBA)
    std::vector<uint32_t> atlasData(m_atlasW * m_atlasH, 0);

    // Second pass: rasterize glyphs into the buffer
    int cursorX = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, loadFlags)) continue;
        FT_Bitmap& bmp = face->glyph->bitmap;

        // Ensure we actually got a grayscale bitmap
        if (bmp.pixel_mode != FT_PIXEL_MODE_GRAY && bmp.pixel_mode != FT_PIXEL_MODE_NONE) {
            LOG_ERROR("Font: Unsupported pixel mode for character " << c);
            continue;
        }

        // Copy FreeType bitmap to atlas.
        // We do NOT flip the rows here. FreeType row 0 (top) goes into Atlas row 0.
        // In OpenGL, Atlas row 0 is V=0 (bottom).
        // Since our projection flips Y (Y=0 is top), mapping the quad's top (rect.y)
        // to V=0 correctly places the top of the glyph at the top of the quad.
        for (unsigned int row = 0; row < bmp.rows; ++row) {
            for (unsigned int col = 0; col < bmp.width; ++col) {
                unsigned char val = bmp.buffer[row * bmp.pitch + col];
                atlasData[(row * m_atlasW) + cursorX + col] = 
                    (static_cast<uint32_t>(val) << 24) | 0x00FFFFFF;
            }
        }

        GlyphMetrics& g = m_glyphs[c];
        g.u0       = (float)cursorX               / m_atlasW;
        g.v0       = 0.0f;
        g.u1       = (float)(cursorX + bmp.width) / m_atlasW;
        g.v1       = (float)bmp.rows               / m_atlasH;
        g.width    = bmp.width;
        g.height   = bmp.rows;
        g.bearingX = face->glyph->bitmap_left;
        g.bearingY = face->glyph->bitmap_top;
        g.advance  = static_cast<int>(face->glyph->advance.x >> 6);

        cursorX += bmp.width + 2;
    }

    glGenTextures(1, &m_atlasID);
    glBindTexture(GL_TEXTURE_2D, m_atlasID);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 m_atlasW, m_atlasH, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
                 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    LOG_INFO("Font loaded (POT RGBA): " << path << " @ " << pixelHeight << "px | atlas "
             << m_atlasW << "x" << m_atlasH);
    return true;
}

} // namespace orf
