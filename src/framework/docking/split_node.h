#pragma once
#include "dock_node.h"

namespace orf {
class SplitNode : public DockNode {
    friend class DockManager;
public:
    SplitNode(SplitAxis axis,
              std::unique_ptr<DockNode> first,
              std::unique_ptr<DockNode> second,
              float ratio = 0.5f);

    DockNodeType type() const override { return DockNodeType::Split; }

    void layout(const Rect& bounds) override;
    void paint (Renderer2D& renderer) override;

    nlohmann::json serialize() const override;

    bool handleMouseMove   (float x, float y)           override;
    bool handleMousePress  (float x, float y, int button) override;
    bool handleMouseRelease(float x, float y, int button) override;

    DockNode* first()  const { return m_first.get(); }
    DockNode* second() const { return m_second.get(); }
    float     ratio()  const { return m_ratio; }
    SplitAxis axis()   const { return m_axis; }

    bool  dividerContains(float x, float y) const;

    // Replace a child (used during drag-and-drop restructuring)
    void setFirst (std::unique_ptr<DockNode> node);
    void setSecond(std::unique_ptr<DockNode> node);

private:
    static constexpr float DIVIDER_THICKNESS = 4.0f;
    static constexpr float DIVIDER_HIT_AREA  = 8.0f; // wider hit zone than visual

    Rect  dividerRect()                     const;

    SplitAxis m_axis;
    std::unique_ptr<DockNode> m_first;
    std::unique_ptr<DockNode> m_second;
    float m_ratio     = 0.5f;  // 0..1, fraction given to first child
    bool  m_dragging  = false;
    float m_dragStart = 0.0f;  // cursor position when drag began
    float m_ratioAtDragStart = 0.5f;
};

} // namespace orf
