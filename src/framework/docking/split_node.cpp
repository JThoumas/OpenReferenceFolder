#include "split_node.h"
#include "framework/theme/theme_manager.h"
#include <algorithm>

namespace orf {

SplitNode::SplitNode(SplitAxis axis,
                     std::unique_ptr<DockNode> first,
                     std::unique_ptr<DockNode> second,
                     float ratio)
    : m_axis(axis)
    , m_first(std::move(first))
    , m_second(std::move(second))
    , m_ratio(std::clamp(ratio, 0.05f, 0.95f))
{}

void SplitNode::layout(const Rect& bounds) {
    m_bounds = bounds;

    if (m_axis == SplitAxis::Horizontal) {
        // First child: left portion, second child: right portion
        float firstW  = (bounds.width - DIVIDER_THICKNESS) * m_ratio;
        float secondW =  bounds.width - DIVIDER_THICKNESS - firstW;
        m_first ->layout({bounds.x,               bounds.y, firstW,  bounds.height});
        m_second->layout({bounds.x + firstW + DIVIDER_THICKNESS, bounds.y, secondW, bounds.height});
    } else {
        // Vertical split: first on top, second on bottom
        float firstH  = (bounds.height - DIVIDER_THICKNESS) * m_ratio;
        float secondH =  bounds.height - DIVIDER_THICKNESS - firstH;
        m_first ->layout({bounds.x, bounds.y,               bounds.width, firstH });
        m_second->layout({bounds.x, bounds.y + firstH + DIVIDER_THICKNESS, bounds.width, secondH});
    }
}

void SplitNode::markDirty() {
    m_first->markDirty();
    m_second->markDirty();
}

Rect SplitNode::dividerRect() const {
    if (m_axis == SplitAxis::Horizontal) {
        float firstW = (m_bounds.width - DIVIDER_THICKNESS) * m_ratio;
        return {m_bounds.x + firstW, m_bounds.y, DIVIDER_THICKNESS, m_bounds.height};
    } else {
        float firstH = (m_bounds.height - DIVIDER_THICKNESS) * m_ratio;
        return {m_bounds.x, m_bounds.y + firstH, m_bounds.width, DIVIDER_THICKNESS};
    }
}

bool SplitNode::dividerContains(float x, float y) const {
    Rect d = dividerRect();
    float hx = DIVIDER_HIT_AREA * 0.5f;
    // Expand hit area around the divider center
    if (m_axis == SplitAxis::Horizontal)
        return x >= d.x - hx && x <= d.x + d.width + hx
            && y >= d.y       && y <= d.y + d.height;
    else
        return x >= d.x       && x <= d.x + d.width
            && y >= d.y - hx  && y <= d.y + d.height + hx;
}

void SplitNode::paint(Renderer2D& renderer) {
    // Paint children first
    m_first ->paint(renderer);
    m_second->paint(renderer);

    // Draw divider — slightly brighter when being dragged
    Color divColor = m_dragging
        ? theme().dividerHovered
        : theme().divider;
    renderer.drawRect(dividerRect(), divColor);
}

bool SplitNode::handleMouseMove(float x, float y) {
    if (m_dragging) {
        // Recompute ratio from cursor delta
        if (m_axis == SplitAxis::Horizontal) {
            float delta = x - m_dragStart;
            float newFirst = (m_bounds.width - DIVIDER_THICKNESS) * m_ratioAtDragStart + delta;
            m_ratio = std::clamp(newFirst / (m_bounds.width - DIVIDER_THICKNESS), 0.05f, 0.95f);
        } else {
            float delta = y - m_dragStart;
            float newFirst = (m_bounds.height - DIVIDER_THICKNESS) * m_ratioAtDragStart + delta;
            m_ratio = std::clamp(newFirst / (m_bounds.height - DIVIDER_THICKNESS), 0.05f, 0.95f);
        }
        layout(m_bounds); // re-layout with new ratio
        return true;
    }
    // Route to children
    return m_first->handleMouseMove(x, y) || m_second->handleMouseMove(x, y);
}

bool SplitNode::handleMousePress(float x, float y, int button) {
    if (button == 0 && dividerContains(x, y)) {
        m_dragging = true;
        m_dragStart = (m_axis == SplitAxis::Horizontal) ? x : y;
        m_ratioAtDragStart = m_ratio;
        return true;
    }
    return m_first->handleMousePress(x, y, button)
        || m_second->handleMousePress(x, y, button);
}

bool SplitNode::handleMouseRelease(float x, float y, int button) {
    if (m_dragging) {
        m_dragging = false;
        return true;
    }
    return m_first->handleMouseRelease(x, y, button)
        || m_second->handleMouseRelease(x, y, button);
}

void SplitNode::setFirst(std::unique_ptr<DockNode> node) {
    m_first = std::move(node);
    layout(m_bounds);
}

void SplitNode::setSecond(std::unique_ptr<DockNode> node) {
    m_second = std::move(node);
    layout(m_bounds);
}

nlohmann::json SplitNode::serialize() const {
    nlohmann::json j;
    j["type"]  = "split";
    j["axis"]  = (m_axis == SplitAxis::Horizontal) ? "horizontal" : "vertical";
    j["ratio"] = m_ratio;
    j["first"] = m_first->serialize();
    j["second"] = m_second->serialize();
    return j;
}

} // namespace orf
