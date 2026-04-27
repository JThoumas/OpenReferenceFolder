#include "dock_node.h"
#include "split_node.h"
#include "panel_node.h"
#include "tab_group_node.h"
#include "framework/widgets/panel.h"
#include "app/widgets/file_browser_widget.h"
#include "app/widgets/display_center_widget.h"
#include "framework/theme/theme_manager.h"

namespace orf {

std::unique_ptr<DockNode> DockNode::deserialize(const nlohmann::json& j, Font& font, Database* db, ThumbnailCache* thumbs) {
    std::string type = j["type"];
    
    if (type == "split") {
        SplitAxis axis = (j["axis"] == "horizontal") ? SplitAxis::Horizontal : SplitAxis::Vertical;
        float ratio = j["ratio"];
        auto first  = deserialize(j["first"], font, db, thumbs);
        auto second = deserialize(j["second"], font, db, thumbs);
        if (!first || !second) return nullptr;
        return std::make_unique<SplitNode>(axis, std::move(first), std::move(second), ratio);
    } else if (type == "panel") {
        std::string title = j["title"];
        std::unique_ptr<Widget> content;

        if (title == "Reference Folder" && db && thumbs) {
            auto browser = std::make_unique<FileBrowserWidget>(*db, *thumbs, font);
            browser->refresh();
            browser->setViewMode(BrowserViewMode::Grid);
            content = std::move(browser);
        } else if (title == "Display Center" && db) {
            content = std::make_unique<DisplayCenterWidget>(font, ThemeManager::get(), *db);
        } else {
            content = std::make_unique<Panel>(Color::transparent());
        }
        
        return std::make_unique<PanelNode>(title, std::move(content), font);
    } else if (type == "tabgroup") {
        auto node = std::make_unique<TabGroupNode>(font);
        node->setActiveTab(j["activeTab"]);
        for (const auto& title : j["tabs"]) {
            // Tab groups in this simple impl also just get empty panels for now
            // unless we add content type info to JSON.
            node->addTab(title, std::make_unique<Panel>(Color::transparent()));
        }
        return node;
    }
    
    return nullptr;
}

} // namespace orf
