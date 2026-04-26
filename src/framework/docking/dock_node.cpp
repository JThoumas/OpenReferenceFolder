#include "dock_node.h"
#include "split_node.h"
#include "panel_node.h"
#include "tab_group_node.h"
#include "framework/widgets/panel.h"

namespace orf {

std::unique_ptr<DockNode> DockNode::deserialize(const nlohmann::json& j, Font& font) {
    std::string type = j["type"];
    
    if (type == "split") {
        SplitAxis axis = (j["axis"] == "horizontal") ? SplitAxis::Horizontal : SplitAxis::Vertical;
        float ratio = j["ratio"];
        auto first  = deserialize(j["first"], font);
        auto second = deserialize(j["second"], font);
        return std::make_unique<SplitNode>(axis, std::move(first), std::move(second), ratio);
    } else if (type == "panel") {
        std::string title = j["title"];
        // For now, reconstruct with a generic panel. 
        // In a real app, you'd have a factory for content.
        return std::make_unique<PanelNode>(title, std::make_unique<Panel>(Color{0.18f, 0.20f, 0.18f, 1.0f}), font);
    } else if (type == "tabgroup") {
        auto node = std::make_unique<TabGroupNode>(font);
        node->setActiveTab(j["activeTab"]);
        for (const auto& title : j["tabs"]) {
            node->addTab(title, std::make_unique<Panel>(Color{0.15f, 0.15f, 0.20f, 1.0f}));
        }
        return node;
    }
    
    return nullptr;
}

} // namespace orf
