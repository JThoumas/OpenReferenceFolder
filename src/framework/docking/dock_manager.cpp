#include "dock_manager.h"

namespace orf {

DockManager::DockManager(Font& font) : m_font(font) {}

void DockManager::setRoot(std::unique_ptr<DockNode> root) {
    m_root = std::move(root);
    
    // Set double-click callbacks recursively
    std::function<void(DockNode*)> setupCallbacks = [&](DockNode* node) {
        if (!node) return;
        if (node->type() == DockNodeType::Panel) {
            static_cast<PanelNode*>(node)->setOnDoubleClick([this](PanelNode* p) {
                floatPanel(p);
            });
        } else if (node->type() == DockNodeType::Split) {
            SplitNode* split = static_cast<SplitNode*>(node);
            setupCallbacks(split->first());
            setupCallbacks(split->second());
        }
    };
    setupCallbacks(m_root.get());

    if (m_root && m_width > 0)
        m_root->layout({0, 0, (float)m_width, (float)m_height});
}

std::unique_ptr<DockNode> DockManager::detachRecursive(std::unique_ptr<DockNode>& node, DockNode* target) {
    if (!node) return nullptr;
    if (node.get() == target) {
        return std::move(node);
    }
    
    if (node->type() == DockNodeType::Split) {
        SplitNode* split = static_cast<SplitNode*>(node.get());
        if (split->m_first.get() == target) {
            auto detached = std::move(split->m_first);
            node = std::move(split->m_second); // Replace split with its other child
            return detached;
        }
        if (split->m_second.get() == target) {
            auto detached = std::move(split->m_second);
            node = std::move(split->m_first); // Replace split with its other child
            return detached;
        }
        
        auto detached = detachRecursive(split->m_first, target);
        if (detached) return detached;
        return detachRecursive(split->m_second, target);
    }
    return nullptr;
}

void DockManager::floatPanel(PanelNode* panel) {
    auto detached = detachRecursive(m_root, panel);
    if (detached) {
        auto panelPtr = std::unique_ptr<PanelNode>(static_cast<PanelNode*>(detached.release()));
        
        auto fw = std::make_unique<FloatingWindow>(
            std::move(panelPtr),
            glfwGetCurrentContext(),
            m_font,
            100, 100, 400, 300
        );
        m_floatingWindows.push_back(std::move(fw));
        
        // Trigger re-layout of main tree
        if (m_root) m_root->layout({0, 0, (float)m_width, (float)m_height});
    }
}

void DockManager::updateFloatingWindows() {
    for (auto it = m_floatingWindows.begin(); it != m_floatingWindows.end();) {
        if ((*it)->shouldClose()) {
            it = m_floatingWindows.erase(it);
        } else {
            (*it)->update();
            ++it;
        }
    }
}

void DockManager::paintFloatingWindows() {
    for (auto& fw : m_floatingWindows) {
        fw->paint();
    }
}

void DockManager::resize(int w, int h) {
    m_width  = w;
    m_height = h;
    if (m_root)
        m_root->layout({0, 0, (float)w, (float)h});
}

void DockManager::paint(Renderer2D& renderer) {
    if (m_root) m_root->paint(renderer);
}

void DockManager::onMouseMove(float x, float y) {
    if (m_root) m_root->handleMouseMove(x, y);
}

void DockManager::onMousePress(float x, float y, int button) {
    if (m_root) m_root->handleMousePress(x, y, button);
}

void DockManager::onMouseRelease(float x, float y, int button) {
    if (m_root) m_root->handleMouseRelease(x, y, button);
}

nlohmann::json DockManager::serialize() const {
    if (m_root) return m_root->serialize();
    return nlohmann::json{};
}

std::optional<SplitAxis> DockManager::hitTestDivider(float x, float y) const {
    if (!m_root) return std::nullopt;
    return hitTestDividerRecursive(m_root.get(), x, y);
}

std::optional<SplitAxis> DockManager::hitTestDividerRecursive(DockNode* node, float x, float y) const {
    if (node->type() == DockNodeType::Split) {
        SplitNode* split = static_cast<SplitNode*>(node);
        
        // Recurse first to find leaf-most hit
        auto hit = hitTestDividerRecursive(split->first(), x, y);
        if (hit) return hit;
        hit = hitTestDividerRecursive(split->second(), x, y);
        if (hit) return hit;
        
        // Check this split node's divider
        if (split->dividerContains(x, y)) {
            return split->axis();
        }
    }
    return std::nullopt;
}

} // namespace orf
