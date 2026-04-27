#include "context_menu.h"
#include "framework/theme/theme_manager.h"
#include <algorithm>

namespace orf {

ContextMenu::ContextMenu(std::vector<MenuItem> items, Font& font)
    : m_items(std::move(items)), m_font(font)
{
    m_visible = false;
}

float ContextMenu::menuHeight() const {
    float h = 0;
    for (const auto& item : m_items) {
        h += item.separator ? SEP_HEIGHT : ITEM_HEIGHT;
    }
    return h + 4.0f; // padding
}

int ContextMenu::itemAt(float y) const {
    float curY = m_bounds.y + 2.0f;
    for (int i = 0; i < (int)m_items.size(); ++i) {
        float h = m_items[i].separator ? SEP_HEIGHT : ITEM_HEIGHT;
        if (y >= curY && y < curY + h) {
            return m_items[i].separator ? -1 : i;
        }
        curY += h;
    }
    return -1;
}

void ContextMenu::show(float x, float y) {
    m_shown = true;
    m_visible = true;
    setBounds({x, y, MENU_WIDTH, menuHeight()});
    markDirty();
}

void ContextMenu::dismiss() {
    m_shown = false;
    m_visible = false;
    markDirty();
}

void ContextMenu::onPaint(Renderer2D& renderer) {
    if (!m_shown) return;

    // Shadow/Border
    renderer.drawRect({m_bounds.x + 2, m_bounds.y + 2, m_bounds.width, m_bounds.height}, {0, 0, 0, 0.3f});
    renderer.drawRect(m_bounds, theme().contextMenuBackground);
    
    float curY = m_bounds.y + 2.0f;
    for (int i = 0; i < (int)m_items.size(); ++i) {
        const auto& item = m_items[i];
        float h = item.separator ? SEP_HEIGHT : ITEM_HEIGHT;

        if (item.separator) {
            renderer.drawRect({m_bounds.x + 4, curY + SEP_HEIGHT/2 - 0.5f, m_bounds.width - 8, 1}, theme().contextMenuSeparator);
        } else {
            bool hovered = (i == m_hoveredIdx);
            if (hovered) {
                renderer.drawRect({m_bounds.x + 2, curY, m_bounds.width - 4, ITEM_HEIGHT}, theme().contextMenuHover);
            }
            
            renderer.drawText(item.label, m_bounds.x + 10, curY + ITEM_HEIGHT * 0.5f + m_font.lineHeight() * 0.35f, 
                               m_font, hovered ? Color::white() : theme().contextMenuText);
        }
        curY += h;
    }
}

void ContextMenu::onMousePress(MouseEvent& e) {
    if (!m_shown) return;

    if (containsPoint(e.x, e.y)) {
        int idx = itemAt(e.y);
        if (idx >= 0 && m_items[idx].action) {
            m_items[idx].action();
        }
    }
    dismiss();
    e.consumed = true;
}

void ContextMenu::onMouseMove(MouseEvent& e) {
    if (!m_shown) return;
    m_hoveredIdx = itemAt(e.y);
    markDirty();
    e.consumed = true;
}

void ContextMenu::onMouseMoveGlobal(float x, float y) {
    if (!m_shown) return;
    if (!containsPoint(x, y)) {
        m_hoveredIdx = -1;
        markDirty();
    }
}

} // namespace orf
