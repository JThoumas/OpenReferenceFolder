#pragma once
#include "dock_node.h"
#include "framework/widgets/widget.h"
#include "framework/renderer/font.h"
#include <string>

namespace orf {

// PanelNode wraps a Widget and adds a title bar.
// This is the leaf of the dock tree.
class PanelNode : public DockNode {
public:
    static constexpr float TITLE_BAR_HEIGHT = 24.0f;

    PanelNode(const std::string& title,
              std::unique_ptr<Widget> content,
              Font& font);

    DockNodeType type() const override { return DockNodeType::Panel; }

    void layout(const Rect& bounds) override;
    void paint (Renderer2D& renderer) override;

    nlohmann::json serialize() const override;

    bool handleMousePress  (float x, float y, int button) override;
    bool handleMouseRelease(float x, float y, int button) override;
    bool handleMouseMove   (float x, float y)             override;

    void setOnDoubleClick(std::function<void(PanelNode*)> callback) { m_onDoubleClick = callback; }

    const std::string& title()   const { return m_title; }
    Widget*            content() const { return m_content.get(); }

    // True when the user is dragging this panel's title bar
    bool isDragging() const { return m_dragging; }

private:
    bool titleBarContains(float x, float y) const;

    std::string            m_title;
    std::unique_ptr<Widget> m_content;
    Font&                  m_font;
    bool                   m_dragging  = false;
    float                  m_dragOffX  = 0.0f;
    float                  m_dragOffY  = 0.0f;

    double                 m_lastPressTime = 0.0;
    std::function<void(PanelNode*)> m_onDoubleClick;
};

} // namespace orf
