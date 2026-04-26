#pragma once
#include "dock_node.h"
#include "split_node.h"
#include "panel_node.h"
#include "tab_group_node.h"
#include "floating_window.h"
#include "framework/renderer/renderer2d.h"
#include "framework/renderer/font.h"
#include <memory>
#include <optional>

namespace orf {

class DockManager {
public:
    explicit DockManager(Font& font);

    // Set the root of the dock tree — called once after layout is constructed
    void setRoot(std::unique_ptr<DockNode> root);

    // Called by Application on resize
    void resize(int w, int h);

    // Called each frame
    void paint(Renderer2D& renderer);

    // Input — Application forwards GLFW events here
    void onMouseMove   (float x, float y);
    void onMousePress  (float x, float y, int button);
    void onMouseRelease(float x, float y, int button);

    void floatPanel(PanelNode* panel);
    void updateFloatingWindows();
    void paintFloatingWindows();

    nlohmann::json serialize() const;

    DockNode* root() const { return m_root.get(); }

    // Helper for cursor feedback (Task 4)
    std::optional<SplitAxis> hitTestDivider(float x, float y) const;

private:
    static std::unique_ptr<DockNode> detachRecursive(std::unique_ptr<DockNode>& node, DockNode* target);
    std::optional<SplitAxis> hitTestDividerRecursive(DockNode* node, float x, float y) const;

    std::unique_ptr<DockNode> m_root;
    std::vector<std::unique_ptr<FloatingWindow>> m_floatingWindows;
    Font& m_font;
    int   m_width  = 0;
    int   m_height = 0;
};

} // namespace orf
