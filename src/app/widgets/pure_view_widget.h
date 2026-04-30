#pragma once
#include "framework/widgets/widget.h"
#include "framework/renderer/font.h"
#include "app/thumbnail/thumbnail_cache.h"
#include <filesystem>
#include <string>

namespace orf {

// PureView is the image viewer widget.
// It has two modes:
//   Sidebar mode  — updates on every file click, no close button
//   Pinned mode   — created on double-click, stays on one file until closed
class PureViewWidget : public Widget {
public:
    enum class Mode { Sidebar, Pinned };

    PureViewWidget(Mode mode, ThumbnailCache& thumbs, Font& font);

    // Load a new file — clears the previous image
    void loadFile(const std::filesystem::path& path);

    std::filesystem::path currentPath() const { return m_path; }
    bool hasFile() const { return !m_path.empty(); }
    Mode mode()   const { return m_mode; }

    void onPaint (Renderer2D& renderer)  override;
    void onResize(int w, int h)          override;
    void onMousePress(MouseEvent& e)     override; // zoom on click

private:
    void paintEmpty (Renderer2D& renderer);
    void paintImage (Renderer2D& renderer, GLuint tex);
    void paintInfo  (Renderer2D& renderer);

    // Compute the centered, aspect-correct draw rect for the image
    Rect fitRect(int imgW, int imgH) const;

    Mode                   m_mode;
    ThumbnailCache&        m_thumbs;
    Font&                  m_font;
    std::filesystem::path  m_path;
    std::string            m_filename;
    std::string            m_fileInfo;

    float m_zoom       = 1.0f;
    float m_panX       = 0.0f;
    float m_panY       = 0.0f;
    bool  m_zoomToFit  = true; // reset to fit on new file load
};

} // namespace orf
