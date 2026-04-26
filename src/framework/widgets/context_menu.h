#pragma once
#include "widget.h"
#include "framework/renderer/font.h"
#include <vector>
#include <string>
#include <functional>

namespace orf {

struct MenuItem {
    std::string           label;
    std::function<void()> action;
    bool                  separator = false; // if true, draws a divider line
};

class ContextMenu : public Widget {
public:
    static constexpr float ITEM_HEIGHT  = 26.0f;
    static constexpr float MENU_WIDTH   = 180.0f;
    static constexpr float SEP_HEIGHT   = 6.0f;

    ContextMenu(std::vector<MenuItem> items, Font& font);

    void show(float x, float y);
    void dismiss();
    bool isShown() const { return m_shown; }

    void onPaint       (Renderer2D& renderer)  override;
    void onMousePress  (MouseEvent& e)          override;
    void onMouseMove   (MouseEvent& e)          override;
    void onMouseMoveGlobal(float x, float y)    override;

private:
    float menuHeight() const;
    int   itemAt(float y) const;

    std::vector<MenuItem> m_items;
    Font&                 m_font;
    bool                  m_shown    = false;
    int                   m_hoveredIdx = -1;
};

} // namespace orf
