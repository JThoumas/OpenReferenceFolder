#pragma once
#include "framework/widgets/widget.h"
#include "framework/renderer/font.h"
#include "app/model/orf_node.h"
#include "app/model/database.h"
#include "app/thumbnail/thumbnail_cache.h"
#include "framework/widgets/context_menu.h"
#include <functional>
#include <optional>

namespace orf {

// View modes for the file browser
enum class BrowserViewMode {
    List,       // Single-column list with small icons
    Grid        // Multi-column grid with large thumbnails
};

class FileBrowserWidget : public Widget {
public:
    static constexpr float ROW_HEIGHT       = 28.0f;
    static constexpr float INDENT_WIDTH     = 16.0f;
    static constexpr float THUMB_GRID_SIZE  = 140.0f; // cell size in grid mode
    static constexpr float THUMB_PADDING    = 8.0f;

    FileBrowserWidget(Database& db, ThumbnailCache& thumbs, Font& font);

    // Reload tree from database
    void refresh();

    void setViewMode(BrowserViewMode mode);

    // Fires when a file node is single-clicked
    void setOnSelect(std::function<void(const ORFNode&)> cb) {
        m_onSelect = std::move(cb);
    }

    // Fires when a file node is double-clicked
    void setOnOpen(std::function<void(const ORFNode&)> cb) {
        m_onOpen = std::move(cb);
    }

    void onPaint       (Renderer2D& renderer)  override;
    void onMousePress  (MouseEvent& e)          override;
    void onScroll      (float delta)            override;
    void onResize      (int w, int h)           override;
    void onMouseMoveGlobal(float x, float y)    override;

private:
    // Flattened list of visible nodes (expanded tree)
    struct FlatNode {
        ORFNode* node;
        int      depth;        // indentation level
    };

    void rebuildFlatList();
    void paintList(Renderer2D& renderer);
    void paintGrid(Renderer2D& renderer);
    void paintScrollbar(Renderer2D& renderer);

    std::vector<MenuItem> buildContextMenu(const ORFNode& node);
    std::vector<MenuItem> buildEmptyContextMenu();

    int  nodeAtY(float y) const; // returns flat list index or -1
    int  nodeAtGrid(float x, float y) const;

    float maxScrollOffset() const;

    Database&        m_db;
    ThumbnailCache&  m_thumbs;
    Font&            m_font;

    std::vector<ORFNode>   m_roots;       // top-level nodes from DB
    std::vector<FlatNode>  m_flatList;    // flattened visible tree

    BrowserViewMode m_viewMode    = BrowserViewMode::Grid;
    float           m_scrollOffset = 0.0f;
    int             m_selectedIdx  = -1;

    double          m_lastClickTime = 0.0;
    int             m_lastClickIdx  = -1;

    std::unique_ptr<ContextMenu> m_contextMenu;

    std::function<void(const ORFNode&)> m_onSelect;
    std::function<void(const ORFNode&)> m_onOpen;
};

} // namespace orf
