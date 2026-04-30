#pragma once
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"
#include <string>
#include <vector>
#include <functional>

namespace orf {

struct MenuEntry {
    std::string label;
    std::function<void()> action;
    bool separator = false;
};

struct Menu {
    std::string label;
    std::vector<MenuEntry> entries;
};

class TopBar {
public:
    explicit TopBar(Font& font);
    ~TopBar() = default;

    void setBounds(const Rect& r);
    Rect bounds() const { return m_bounds; }

    void addMenu(Menu menu);

    void paint(Renderer2D& renderer);
    void onMouseMove(float x, float y);
    void onMousePress(float x, float y, int button);
    void onMouseRelease(float x, float y, int button);

    void dismissMenu();

private:
    Font& m_font;
    Rect m_bounds{};
    std::vector<Menu> m_menus;
};

} // namespace orf
