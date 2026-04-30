#include "top_bar.h"
#include "framework/theme/theme_manager.h"
#include <algorithm>

namespace orf {

static constexpr float MENU_LABEL_PADDING = 12.0f;
static constexpr float ENTRY_HEIGHT       = 26.0f;
static constexpr float DROPDOWN_WIDTH     = 200.0f;
static constexpr float SEPARATOR_H        = 8.0f;

TopBar::TopBar(Font& font) : m_font(font) {}

void TopBar::setBounds(const Rect& r) { m_bounds = r; }

void TopBar::addMenu(Menu menu) {
    m_menus.push_back(std::move(menu));
}

Rect TopBar::menuLabelRect(int idx) const {
    float x = m_bounds.x + 40.0f; // Offset for ORF logo
    for (int i = 0; i < idx; ++i) {
        // Approximate: label length * ~7.5px per character + padding
        x += m_menus[i].label.size() * 7.5f + MENU_LABEL_PADDING * 2;
    }
    float w = m_menus[idx].label.size() * 7.5f + MENU_LABEL_PADDING * 2;
    return {x, m_bounds.y, w, m_bounds.height};
}

int TopBar::menuAt(float x, float y) const {
    if (y < m_bounds.y || y >= m_bounds.y + m_bounds.height) return -1;
    for (int i = 0; i < (int)m_menus.size(); ++i) {
        Rect r = menuLabelRect(i);
        if (x >= r.x && x < r.x + r.width) return i;
    }
    return -1;
}

int TopBar::entryAt(float x, float y) const {
    if (m_openMenu < 0) return -1;
    Rect anchor = menuLabelRect(m_openMenu);
    float dropX = anchor.x;
    float dropY = m_bounds.y + m_bounds.height;
    if (x < dropX || x >= dropX + DROPDOWN_WIDTH) return -1;

    float curY = dropY;
    for (int i = 0; i < (int)m_menus[m_openMenu].entries.size(); ++i) {
        float h = m_menus[m_openMenu].entries[i].separator
                  ? SEPARATOR_H : ENTRY_HEIGHT;
        if (y >= curY && y < curY + h && !m_menus[m_openMenu].entries[i].separator)
            return i;
        curY += h;
    }
    return -1;
}

void TopBar::paint(Renderer2D& renderer) {
    renderer.drawRect(m_bounds, theme().titleBarBackground);

    // ORF logo / app name
    renderer.drawText("ORF",
                       m_bounds.x + 8.0f,
                       m_bounds.y + m_bounds.height * 0.5f + m_font.lineHeight() * 0.4f,
                       m_font, theme().accent);

    // Menu labels
    for (int i = 0; i < (int)m_menus.size(); ++i) {
        Rect r   = menuLabelRect(i);
        bool open    = (m_openMenu  == i);
        bool hovered = (m_hoveredMenu == i);

        if (open || hovered)
            renderer.drawRect(r, theme().tabActive);

        renderer.drawText(m_menus[i].label,
                          r.x + MENU_LABEL_PADDING,
                          r.y + r.height * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font,
                          open ? theme().tabActiveText : theme().textPrimary);
    }

    // Bottom separator line
    renderer.drawRect({m_bounds.x, m_bounds.y + m_bounds.height - 1.0f,
                       m_bounds.width, 1.0f},
                       theme().panelBorder);

    if (m_openMenu >= 0)
        paintDropdown(renderer);
}

void TopBar::paintDropdown(Renderer2D& renderer) {
    Rect anchor = menuLabelRect(m_openMenu);
    float x = anchor.x;
    float y = m_bounds.y + m_bounds.height;

    // Measure total dropdown height
    float totalH = 0.0f;
    for (auto& e : m_menus[m_openMenu].entries)
        totalH += e.separator ? SEPARATOR_H : ENTRY_HEIGHT;

    // Background + shadow
    renderer.drawRect({x - 1.0f, y - 1.0f, DROPDOWN_WIDTH + 2.0f, totalH + 2.0f},
                       theme().panelBorder);
    renderer.drawRect({x, y, DROPDOWN_WIDTH, totalH},
                       theme().contextMenuBackground);

    float curY = y;
    for (int i = 0; i < (int)m_menus[m_openMenu].entries.size(); ++i) {
        auto& entry = m_menus[m_openMenu].entries[i];

        if (entry.separator) {
            renderer.drawRect({x + 8.0f, curY + SEPARATOR_H * 0.5f - 0.5f,
                               DROPDOWN_WIDTH - 16.0f, 1.0f},
                               theme().contextMenuSeparator);
            curY += SEPARATOR_H;
            continue;
        }

        bool hovered = (m_hoveredEntry == i);
        if (hovered)
            renderer.drawRect({x, curY, DROPDOWN_WIDTH, ENTRY_HEIGHT},
                               theme().contextMenuHover);

        renderer.drawText(entry.label,
                          x + 12.0f,
                          curY + ENTRY_HEIGHT * 0.5f + m_font.lineHeight() * 0.4f,
                          m_font,
                          hovered ? theme().browserSelectedText
                                  : theme().contextMenuText);
        curY += ENTRY_HEIGHT;
    }
}

void TopBar::onMousePress(float x, float y, int button) {
    if (button != 0) return;

    // Click inside bar — open or close menu
    int clicked = menuAt(x, y);
    if (clicked >= 0) {
        m_openMenu = (m_openMenu == clicked) ? -1 : clicked;
        return;
    }

    // Click inside open dropdown — fire action
    int entry = entryAt(x, y);
    if (entry >= 0 && m_openMenu >= 0) {
        auto& e = m_menus[m_openMenu].entries[entry];
        if (e.action) e.action();
        m_openMenu = -1;
        return;
    }

    // Click outside — dismiss
    dismissMenu();
}

void TopBar::onMouseRelease(float x, float y, int button) {}

void TopBar::onMouseMove(float x, float y) {
    m_hoveredMenu  = menuAt(x, y);
    m_hoveredEntry = entryAt(x, y);

    // Hover-open: if a menu is already open and we hover a different label,
    // switch to it immediately (standard menu bar behaviour)
    if (m_openMenu >= 0 && m_hoveredMenu >= 0 && m_hoveredMenu != m_openMenu)
        m_openMenu = m_hoveredMenu;
}

void TopBar::dismissMenu() {
    m_openMenu     = -1;
    m_hoveredMenu  = -1;
    m_hoveredEntry = -1;
}

} // namespace orf
