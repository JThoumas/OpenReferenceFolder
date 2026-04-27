#pragma once
#include "dock_node.h"
#include "framework/widgets/widget.h"
#include "framework/renderer/font.h"
#include <vector>
#include <string>
#include <memory>

namespace orf {

struct TabEntry {
    std::string             title;
    std::unique_ptr<Widget> content;
};

class TabGroupNode : public DockNode {
public:
    static constexpr float TAB_BAR_HEIGHT = 28.0f;
    static constexpr float TAB_MIN_WIDTH  = 80.0f;

    explicit TabGroupNode(Font& font);

    DockNodeType type() const override { return DockNodeType::TabGroup; }

    void addTab(const std::string& title, std::unique_ptr<Widget> content);
    void setActiveTab(int index);
    int  activeTab() const { return m_activeTab; }
    int  tabCount()  const { return static_cast<int>(m_tabs.size()); }

    void layout(const Rect& bounds) override;
    void paint (Renderer2D& renderer) override;
    void markDirty() override;
    
    nlohmann::json serialize() const override;

    bool handleMousePress(float x, float y, int button) override;

private:
    int  tabAt(float x, float y) const; // returns tab index or -1
    Rect tabRect(int index)      const;

    std::vector<TabEntry> m_tabs;
    int                   m_activeTab = 0;
    Font&                 m_font;
};

} // namespace orf
