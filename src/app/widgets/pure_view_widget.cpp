#include "pure_view_widget.h"
#include "framework/theme/theme_manager.h"
#include "framework/core/log.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

namespace orf {

PureViewWidget::PureViewWidget(Mode mode, ThumbnailCache& thumbs, Font& font)
    : m_mode(mode), m_thumbs(thumbs), m_font(font)
{}

void PureViewWidget::loadFile(const std::filesystem::path& path) {
    m_path     = path;
    m_filename = path.filename().string();
    m_zoom     = 1.0f;
    m_panX     = 0.0f;
    m_panY     = 0.0f;
    m_zoomToFit = true;

    // Build info string from filesystem metadata
    std::error_code ec;
    auto size = std::filesystem::file_size(path, ec);
    if (!ec) {
        char buf[64];
        if (size < 1024)
            snprintf(buf, sizeof(buf), "%llu B", (unsigned long long)size);
        else if (size < 1024 * 1024)
            snprintf(buf, sizeof(buf), "%.1f KB", size / 1024.0);
        else
            snprintf(buf, sizeof(buf), "%.2f MB", size / (1024.0 * 1024.0));
        m_fileInfo = std::string(buf) + "  " + path.extension().string();
    }

    // Request the thumbnail — it will decode async
    m_thumbs.request(path);
    markDirty();
    LOG_INFO("PureView: loading " << m_filename);
}

void PureViewWidget::onResize(int, int) { markDirty(); }

void PureViewWidget::onMousePress(MouseEvent& e) {
    if (e.button == 0 && hasFile()) {
        // Left click toggles fit/1:1 zoom
        m_zoomToFit = !m_zoomToFit;
        if (m_zoomToFit) {
            m_zoom = 1.0f;
            m_panX = 0.0f;
            m_panY = 0.0f;
        }
        markDirty();
        e.consumed = true;
    }
}

Rect PureViewWidget::fitRect(int imgW, int imgH) const {
    if (imgW <= 0 || imgH <= 0)
        return m_bounds;

    float viewW = m_bounds.width;
    float viewH = m_bounds.height - 30.0f; // reserve bottom for info bar
    float scale = std::min(viewW / imgW, viewH / imgH);

    float dw = imgW * scale;
    float dh = imgH * scale;
    float dx = m_bounds.x + (viewW - dw) * 0.5f;
    float dy = m_bounds.y + (viewH - dh) * 0.5f;
    return {dx, dy, dw, dh};
}

void PureViewWidget::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    // Background — slightly darker than panel for contrast
    renderer.drawRect(m_bounds,
                       {theme().windowBackground.r * 0.85f,
                        theme().windowBackground.g * 0.85f,
                        theme().windowBackground.b * 0.85f,
                        1.0f});

    if (!hasFile()) {
        paintEmpty(renderer);
        return;
    }

    GLuint tex = m_thumbs.request(m_path);
    if (tex != 0)
        paintImage(renderer, tex);

    paintInfo(renderer);
}

void PureViewWidget::paintEmpty(Renderer2D& renderer) {
    // Centered instructions
    float cx = m_bounds.x + m_bounds.width  * 0.5f;
    float cy = m_bounds.y + m_bounds.height * 0.5f;

    renderer.drawText("No file selected",
                       cx - 70.0f, cy,
                       m_font, theme().textSecondary);

    if (m_mode == Mode::Sidebar) {
        renderer.drawText("Click a file in the browser",
                          cx - 100.0f, cy + m_font.lineHeight() + 4.0f,
                          m_font, theme().textDisabled);
    } else {
        renderer.drawText("Double-click a file to pin it here",
                          cx - 120.0f, cy + m_font.lineHeight() + 4.0f,
                          m_font, theme().textDisabled);
    }
}

void PureViewWidget::paintImage(Renderer2D& renderer, GLuint tex) {
    // For now use the thumbnail texture at fit size
    // Phase 8 loads full-resolution via a separate pipeline
    Rect r = fitRect(
        ThumbnailCache::THUMB_SIZE,
        ThumbnailCache::THUMB_SIZE
    );
    renderer.drawTexturedRect(r, tex);
}

void PureViewWidget::paintInfo(Renderer2D& renderer) {
    // Info bar at the bottom of the view area
    float barY = m_bounds.y + m_bounds.height - 28.0f;
    renderer.drawRect({m_bounds.x, barY, m_bounds.width, 28.0f},
                       {0.0f, 0.0f, 0.0f, 0.35f});

    renderer.drawText(m_filename,
                       m_bounds.x + 8.0f,
                       barY + 18.0f,
                       m_font, theme().textPrimary);

    if (!m_fileInfo.empty()) {
        renderer.drawText(m_fileInfo,
                           m_bounds.x + m_bounds.width - 140.0f,
                           barY + 18.0f,
                           m_font, theme().textSecondary);
    }
}

} // namespace orf
