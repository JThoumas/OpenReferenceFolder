#pragma once
#include "framework/renderer/renderer2d.h"
#include <nlohmann/json.hpp>
#include <memory>

namespace orf {

enum class DockNodeType { Split, TabGroup, Panel };
enum class SplitAxis   { Horizontal, Vertical };

class DockNode {
public:
    virtual ~DockNode() = default;

    virtual DockNodeType type()  const = 0;
    virtual void layout(const Rect& bounds) = 0;
    virtual void paint (Renderer2D& renderer) = 0;

    virtual nlohmann::json serialize() const = 0;
    static std::unique_ptr<DockNode> deserialize(const nlohmann::json& j, Font& font);

    // Input — returns true if the node consumed the event
    virtual bool handleMouseMove   (float x, float y) { return false; }
    virtual bool handleMousePress  (float x, float y, int button) { return false; }
    virtual bool handleMouseRelease(float x, float y, int button) { return false; }

    Rect bounds() const { return m_bounds; }

protected:
    Rect m_bounds{};
};

} // namespace orf
