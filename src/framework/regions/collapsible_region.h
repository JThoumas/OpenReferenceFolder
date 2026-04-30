#pragma once
#include "framework/widgets/widget.h"
#include "framework/renderer/font.h"
#include <string>
#include <memory>

namespace orf {

class TabGroupNode;

class CollapsibleRegion {
public:
    enum class Side { Left, Right, Bottom };
    CollapsibleRegion(Side side, Font& font);
    ~CollapsibleRegion();

    void setBounds(const Rect& r);
    Rect bounds() const { return m_bounds; }

    bool isCollapsed() const { return m_collapsed; }
    void setCollapsed(bool v);
    void toggleCollapsed();

    void addTab(const std::string& title, std::unique_ptr<Widget> content);

    void paint(Renderer2D& renderer);
    void onMouseMove(float x, float y);
    void onMousePress(float x, float y, int button);
    void onMouseRelease(float x, float y, int button);
    void onScroll(float delta);

private:
    Side m_side;
    Font& m_font;
    Rect m_bounds{};
    bool m_collapsed = false;
    std::unique_ptr<TabGroupNode> m_content;
};

} // namespace orf
