#pragma once
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"
#include <memory>
#include <string>

namespace orf {

class TabGroupNode;
class Widget;

class CenterWorkArea {
public:
    explicit CenterWorkArea(Font& font);
    ~CenterWorkArea();

    void setBounds(const Rect& r);
    Rect bounds() const { return m_bounds; }

    void addTab(const std::string& title, std::unique_ptr<Widget> content);
    bool focusTab(const std::string& title);
    bool closeTab(const std::string& title);

    void paint(Renderer2D& renderer);
    void onMouseMove(float x, float y);
    void onMousePress(float x, float y, int button);
    void onMouseRelease(float x, float y, int button);
    void onScroll(float delta);
    void onKey(int key, int action);

private:
    Font& m_font;
    Rect m_bounds{};
    std::unique_ptr<TabGroupNode> m_tabs;
};

} // namespace orf
