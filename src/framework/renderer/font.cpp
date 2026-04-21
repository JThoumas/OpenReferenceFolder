#include "font.h"
#include "framework/core/log.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <cstring>
#include <algorithm>

namespace orf {

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

    // First pass: measure total atlas dimensions
    // Pack all 128 ASCII glyphs into a single horizontal strip
    int totalWidth = 0, maxHeight = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        totalWidth += face->glyph->bitmap.width + 2; // +2px padding
        maxHeight   = std::max(maxHeight, (int)face->glyph->bitmap.rows);
    }
    m_atlasW = totalWidth;
    m_atlasH = maxHeight;

    // Allocate atlas CPU buffer (single channel — FreeType outputs greyscale)
    std::vector<unsigned char> atlasData(m_atlasW * m_atlasH, 0);

    // Second pass: rasterize glyphs into the buffer
    int cursorX = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
        FT_Bitmap& bmp = face->glyph->bitmap;

        for (unsigned int row = 0; row < bmp.rows; ++row) {
            memcpy(
                atlasData.data() + row * m_atlasW + cursorX,
                bmp.buffer        + row * bmp.pitch,
                bmp.width
            );
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
        g.advance  = static_cast<int>(face->glyph->advance.x >> 6); // 26.6 fixed point

        cursorX += bmp.width + 2;
    }

    // Upload atlas as an R8 texture (single red channel)
    glGenTextures(1, &m_atlasID);
    glBindTexture(GL_TEXTURE_2D, m_atlasID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 m_atlasW, m_atlasH, 0,
                 GL_RED, GL_UNSIGNED_BYTE, atlasData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    LOG_INFO("Font loaded: " << path << " @ " << pixelHeight << "px | atlas "
             << m_atlasW << "x" << m_atlasH);
    return true;
}

} // namespace orf
