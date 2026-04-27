#include "file_browser_widget.h"
#include "framework/core/log.h"
#include "platform/file_dialog.h"
#include "framework/theme/theme_manager.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

namespace orf {

FileBrowserWidget::FileBrowserWidget(Database& db,
                                     ThumbnailCache& thumbs,
                                     Font& font)
    : m_db(db), m_thumbs(thumbs), m_font(font)
{}

void FileBrowserWidget::refresh() {
    m_roots = m_db.loadTree();
    rebuildFlatList();
    markDirty();
}

void FileBrowserWidget::setViewMode(BrowserViewMode mode) {
    m_viewMode = mode;
    markDirty();
}

void FileBrowserWidget::rebuildFlatList() {
    m_flatList.clear();

    std::function<void(std::vector<ORFNode>&, int)> flatten =
        [&](std::vector<ORFNode>& nodes, int depth) {
            for (auto& node : nodes) {
                m_flatList.push_back({&node, depth});
                if (node.isFolder() && node.isExpanded)
                    flatten(node.children, depth + 1);
            }
        };

    flatten(m_roots, 0);
}

float FileBrowserWidget::maxScrollOffset() const {
    float contentH = (m_viewMode == BrowserViewMode::List)
        ? m_flatList.size() * ROW_HEIGHT
        : std::ceil((float)m_flatList.size()
                    / (float)std::max(1, (int)(m_bounds.width / THUMB_GRID_SIZE)))
          * THUMB_GRID_SIZE;
    return std::max(0.0f, contentH - m_bounds.height);
}

void FileBrowserWidget::onResize(int, int) {
    m_scrollOffset = std::clamp(m_scrollOffset, 0.0f, maxScrollOffset());
    markDirty();
}

void FileBrowserWidget::onScroll(float delta) {
    m_scrollOffset = std::clamp(
        m_scrollOffset - delta * 24.0f,
        0.0f, maxScrollOffset()
    );
    markDirty();
}

void FileBrowserWidget::onPaint(Renderer2D& renderer) {
    if (!m_visible) return;

    // Background
    renderer.drawRect(m_bounds, theme().browserBackground);

    renderer.pushClip(m_bounds);

    if (m_viewMode == BrowserViewMode::List)
        paintList(renderer);
    else
        paintGrid(renderer);

    renderer.popClip();

    paintScrollbar(renderer);
    
    if (m_contextMenu && m_contextMenu->isVisible()) {
        m_contextMenu->onPaint(renderer);
    }

    clearDirty();
}

void FileBrowserWidget::paintList(Renderer2D& renderer) {
    for (int i = 0; i < (int)m_flatList.size(); ++i) {
        const FlatNode& fn = m_flatList[i];

        float y = m_bounds.y + i * ROW_HEIGHT - m_scrollOffset;
        if (y + ROW_HEIGHT < m_bounds.y) continue;
        if (y > m_bounds.y + m_bounds.height) break;

        // Row background — highlight selected
        bool selected = (i == m_selectedIdx);
        if (selected)
            renderer.drawRect(
                {m_bounds.x, y, m_bounds.width, ROW_HEIGHT},
                theme().browserSelected
            );
        else if (i % 2 == 0)
            renderer.drawRect(
                {m_bounds.x, y, m_bounds.width, ROW_HEIGHT},
                theme().browserRowOdd
            );

        float indent = m_bounds.x + fn.depth * INDENT_WIDTH + 6.0f;

        // Folder expand arrow
        if (fn.node->isFolder()) {
            renderer.drawText(
                fn.node->isExpanded ? "▼" : "▶",
                indent, y + ROW_HEIGHT * 0.5f + m_font.lineHeight() * 0.35f,
                m_font, theme().textSecondary
            );
            indent += 14.0f;
        }

        // Thumbnail (small, in list mode)
        if (fn.node->type == NodeType::File && fn.node->hasRealPath()) {
            GLuint tex = m_thumbs.request(fn.node->realPath);
            if (tex != 0) {
                float iconSize = ROW_HEIGHT - 4.0f;
                renderer.drawTexturedRect(
                    {indent, y + 2.0f, iconSize, iconSize},
                    tex
                );
                indent += iconSize + 4.0f;
            }
        }

        // Display name
        renderer.drawText(
            fn.node->displayName,
            indent,
            y + ROW_HEIGHT * 0.5f + m_font.lineHeight() * 0.35f,
            m_font,
            selected ? theme().browserSelectedText : theme().browserText
        );
    }
}

void FileBrowserWidget::paintGrid(Renderer2D& renderer) {
    int cols = std::max(1, (int)(m_bounds.width / THUMB_GRID_SIZE));
    float cellW = m_bounds.width / (float)cols;
    float cellH = THUMB_GRID_SIZE;

    for (int i = 0; i < (int)m_flatList.size(); ++i) {
        const FlatNode& fn = m_flatList[i];

        int col = i % cols;
        int row = i / cols;

        float x = m_bounds.x + col * cellW;
        float y = m_bounds.y + row * cellH - m_scrollOffset;

        if (y + cellH < m_bounds.y) continue;
        if (y > m_bounds.y + m_bounds.height) break;

        // Cell background
        bool selected = (i == m_selectedIdx);
        if (selected)
            renderer.drawRect(
                {x, y, cellW, cellH},
                theme().browserSelected
            );

        // Thumbnail
        float thumbSize = cellW - THUMB_PADDING * 2;
        float thumbY    = y + THUMB_PADDING;

        if (fn.node->type == NodeType::File && fn.node->hasRealPath()) {
            GLuint tex = m_thumbs.request(fn.node->realPath);
            if (tex != 0) {
                renderer.drawTexturedRect(
                    {x + THUMB_PADDING, thumbY, thumbSize, thumbSize - 20.0f},
                    tex
                );
            }
        } else {
            // Folder icon — colored rect placeholder until Phase 6 icons
            renderer.drawRect(
                {x + THUMB_PADDING, thumbY, thumbSize, thumbSize - 20.0f},
                theme().folderIcon
            );
        }

        // Label below thumbnail
        renderer.drawText(
            fn.node->displayName,
            x + THUMB_PADDING,
            y + cellH - 14.0f,
            m_font,
            theme().browserText
        );
    }
}

void FileBrowserWidget::paintScrollbar(Renderer2D& renderer) {
    float total = maxScrollOffset() + m_bounds.height;
    if (total <= m_bounds.height) return;

    float trackW = 6.0f;
    float trackX = m_bounds.x + m_bounds.width - trackW;
    float trackH = m_bounds.height;

    renderer.drawRect({trackX, m_bounds.y, trackW, trackH},
                       theme().scrollbarTrack);

    float thumbH = (m_bounds.height / total) * trackH;
    float thumbY = m_bounds.y
                 + (m_scrollOffset / maxScrollOffset()) * (trackH - thumbH);

    renderer.drawRect({trackX, thumbY, trackW, thumbH},
                       theme().scrollbarThumb);
}

int FileBrowserWidget::nodeAtY(float y) const {
    float relY = y - m_bounds.y + m_scrollOffset;
    int idx = (int)(relY / ROW_HEIGHT);
    if (idx < 0 || idx >= (int)m_flatList.size()) return -1;
    return idx;
}

int FileBrowserWidget::nodeAtGrid(float x, float y) const {
    int cols = std::max(1, (int)(m_bounds.width / THUMB_GRID_SIZE));
    float cellW = m_bounds.width / (float)cols;
    float cellH = THUMB_GRID_SIZE;

    int col = (int)((x - m_bounds.x) / cellW);
    int row = (int)((y - m_bounds.y + m_scrollOffset) / cellH);
    int idx = row * cols + col;

    if (idx < 0 || idx >= (int)m_flatList.size()) return -1;
    return idx;
}

void FileBrowserWidget::onMousePress(MouseEvent& e) {
    if (m_contextMenu && m_contextMenu->isVisible()) {
        m_contextMenu->onMousePress(e);
        if (e.consumed) return;
    }

    int idx = (m_viewMode == BrowserViewMode::List)
        ? nodeAtY(e.y)
        : nodeAtGrid(e.x, e.y);

    if (e.button == 1) { // Right click
        std::vector<MenuItem> items;
        if (idx >= 0) {
            items = buildContextMenu(*m_flatList[idx].node);
        } else {
            items = buildEmptyContextMenu();
        }
        
        if (!items.empty()) {
            m_contextMenu = std::make_unique<ContextMenu>(items, m_font);
            m_contextMenu->show(e.x, e.y);
            e.consumed = true;
            markDirty();
        }
        return;
    }

    if (idx < 0) {
        m_selectedIdx = -1;
        markDirty();
        return;
    }

    FlatNode& fn = m_flatList[idx];

    // Double-click detection
    double now = glfwGetTime();
    bool doubleClick = (idx == m_lastClickIdx && now - m_lastClickTime < 0.35);
    m_lastClickIdx  = idx;
    m_lastClickTime = now;

    m_selectedIdx = idx;
    markDirty();

    if (fn.node->isFolder()) {
        if (doubleClick || m_viewMode == BrowserViewMode::List) {
            // Toggle expand in list mode on single click; grid needs double
            fn.node->isExpanded = !fn.node->isExpanded;
            rebuildFlatList();
        }
    } else {
        if (m_onSelect) m_onSelect(*fn.node);
        if (doubleClick && m_onOpen) m_onOpen(*fn.node);
    }

    e.consumed = true;
}

void FileBrowserWidget::onMouseMoveGlobal(float x, float y) {
    if (m_contextMenu && m_contextMenu->isVisible()) {
        m_contextMenu->onMouseMoveGlobal(x, y);
    }
}

std::vector<MenuItem> FileBrowserWidget::buildContextMenu(const ORFNode& node) {
    std::vector<MenuItem> items;

    items.push_back({"Delete from ORF", [this, node]() {
        m_db.deleteNode(node.id);
        refresh();
    }});

    items.push_back({"", {}, true}); // separator
    
    auto moreItems = buildEmptyContextMenu();
    items.insert(items.end(), moreItems.begin(), moreItems.end());

    return items;
}

std::vector<MenuItem> FileBrowserWidget::buildEmptyContextMenu() {
    std::vector<MenuItem> items;

    items.push_back({"Add Files...", [this]() {
        auto paths = platform::openFileDialog("Add Files to ORF", true);
        for (auto& p : paths) {
            ORFNode node;
            node.type        = NodeType::File;
            node.displayName = std::filesystem::path(p).filename().string();
            node.realPath    = p;
            m_db.saveNode(node);
        }
        refresh();
    }});

    items.push_back({"Add Folder...", [this]() {
        auto path = platform::openFolderDialog("Add Folder to ORF");
        if (path) {
            ORFNode node;
            node.type        = NodeType::RealFolder;
            node.displayName = std::filesystem::path(*path).filename().string();
            node.realPath    = *path;
            m_db.saveNode(node);
            refresh();
        }
    }});

    return items;
}

} // namespace orf
