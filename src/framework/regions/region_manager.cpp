#include "region_manager.h"
#include "collapsible_region.h"
#include "center_work_area.h"
#include "top_bar.h"
#include "framework/core/log.h"
#include <algorithm>

namespace orf {

static constexpr float GRIP_HIT = 6.0f; // px either side of border = resize

RegionManager::RegionManager(Font& font) : m_font(font) {}
RegionManager::~RegionManager() = default;

bool RegionManager::init(int w, int h) {
    m_vpWidth  = w;
    m_vpHeight = h;

    m_topBar = std::make_unique<TopBar>(m_font);
    m_left   = std::make_unique<CollapsibleRegion>(
                   CollapsibleRegion::Side::Left,   m_font);
    m_right  = std::make_unique<CollapsibleRegion>(
                   CollapsibleRegion::Side::Right,  m_font);
    m_bottom = std::make_unique<CollapsibleRegion>(
                   CollapsibleRegion::Side::Bottom, m_font);
    m_center = std::make_unique<CenterWorkArea>(m_font);

    recalculateBounds();
    LOG_INFO("RegionManager initialized " << w << "x" << h);
    return true;
}

void RegionManager::recalculateBounds() {
    float W = (float)m_vpWidth;
    float H = (float)m_vpHeight;
    auto& L = m_layout;

    float leftW   = m_left->isCollapsed()   ? L.collapsedSize : L.leftWidth;
    float rightW  = m_right->isCollapsed()  ? L.collapsedSize : L.rightWidth;
    float bottomH = m_bottom->isCollapsed() ? L.collapsedSize : L.bottomHeight;

    float topY    = L.topBarHeight;
    float midH    = H - topY - bottomH;
    float centerX = leftW;
    float centerW = W - leftW - rightW;

    m_topBar->setBounds ({0,        0,    W,       L.topBarHeight});
    m_left  ->setBounds ({0,        topY, leftW,   midH           });
    m_right ->setBounds ({W-rightW, topY, rightW,  midH           });
    m_bottom->setBounds ({leftW,    H-bottomH, centerW, bottomH   });
    m_center->setBounds ({centerX,  topY, centerW, midH           });
}

void RegionManager::resize(int w, int h) {
    m_vpWidth  = w;
    m_vpHeight = h;
    recalculateBounds();
}

void RegionManager::paint(Renderer2D& renderer) {
    // Draw region backgrounds so no gaps appear during resize
    renderer.drawRect({0,0,(float)m_vpWidth,(float)m_vpHeight},
                       theme().windowBackground);

    m_topBar->paint(renderer);
    m_left  ->paint(renderer);
    m_right ->paint(renderer);
    m_bottom->paint(renderer);
    m_center->paint(renderer);
}

bool RegionManager::gripContains(float x, float y, bool horiz) const {
    // Vertical grips (left/right region borders)
    if (!horiz) {
        float leftEdge  = m_left ->bounds().x + m_left ->bounds().width;
        float rightEdge = m_right->bounds().x;
        float topY      = m_layout.topBarHeight;
        float botY      = (float)m_vpHeight - (m_bottom->isCollapsed() ? m_layout.collapsedSize : m_layout.bottomHeight);
        
        if (y < topY || y > botY) return false;
        
        if (!m_left->isCollapsed() && std::abs(x - leftEdge) < GRIP_HIT) return true;
        if (!m_right->isCollapsed() && std::abs(x - rightEdge) < GRIP_HIT) return true;
        return false;
    }
    
    // Horizontal grip (bottom region top border)
    float bottomEdge = m_bottom->bounds().y;
    float leftEdge   = m_left->bounds().width;
    float rightEdge  = (float)m_vpWidth - m_right->bounds().width;
    
    if (x < leftEdge || x > rightEdge) return false;
    return (std::abs(y - bottomEdge) < GRIP_HIT);
}

void RegionManager::onMousePress(float x, float y, int button) {
    if (button != 0) { goto route; }

    {
        float leftEdge  = m_left ->bounds().x + m_left ->bounds().width;
        float rightEdge = m_right->bounds().x;
        float bottomEdge= m_bottom->bounds().y;

        if (!m_left->isCollapsed() && std::abs(x - leftEdge) < GRIP_HIT) {
            m_activeGrip     = GripTarget::Left;
            m_gripDragStart  = x;
            m_gripSizeAtDrag = m_layout.leftWidth;
            return;
        }
        if (!m_right->isCollapsed() && std::abs(x - rightEdge) < GRIP_HIT) {
            m_activeGrip     = GripTarget::Right;
            m_gripDragStart  = x;
            m_gripSizeAtDrag = m_layout.rightWidth;
            return;
        }
        if (!m_bottom->isCollapsed() && std::abs(y - bottomEdge) < GRIP_HIT) {
            m_activeGrip     = GripTarget::Bottom;
            m_gripDragStart  = y;
            m_gripSizeAtDrag = m_layout.bottomHeight;
            return;
        }
    }

route:
    m_topBar->onMousePress(x, y, button);
    m_left  ->onMousePress(x, y, button);
    m_right ->onMousePress(x, y, button);
    m_bottom->onMousePress(x, y, button);
    m_center->onMousePress(x, y, button);
}

void RegionManager::onMouseRelease(float x, float y, int button) {
    if (m_activeGrip != GripTarget::None) {
        m_activeGrip = GripTarget::None;
        return;
    }
    m_topBar->onMouseRelease(x, y, button);
    m_left  ->onMouseRelease(x, y, button);
    m_right ->onMouseRelease(x, y, button);
    m_bottom->onMouseRelease(x, y, button);
    m_center->onMouseRelease(x, y, button);
}

void RegionManager::onMouseMove(float x, float y) {
    if (m_activeGrip != GripTarget::None) {
        float delta = 0.0f;
        float minS  = m_layout.minRegionSize;
        float maxS  = 0.0f;

        switch (m_activeGrip) {
            case GripTarget::Left:
                delta = x - m_gripDragStart;
                maxS  = (float)m_vpWidth * 0.5f;
                m_layout.leftWidth = std::clamp(m_gripSizeAtDrag + delta, minS, maxS);
                break;
            case GripTarget::Right:
                delta = m_gripDragStart - x;
                maxS  = (float)m_vpWidth * 0.5f;
                m_layout.rightWidth = std::clamp(m_gripSizeAtDrag + delta, minS, maxS);
                break;
            case GripTarget::Bottom:
                delta = m_gripDragStart - y;
                maxS  = (float)m_vpHeight * 0.4f;
                m_layout.bottomHeight = std::clamp(m_gripSizeAtDrag + delta, minS, maxS);
                break;
            default: break;
        }
        recalculateBounds();
        return;
    }
    m_topBar->onMouseMove(x, y);
    m_left  ->onMouseMove(x, y);
    m_right ->onMouseMove(x, y);
    m_bottom->onMouseMove(x, y);
    m_center->onMouseMove(x, y);
}

void RegionManager::onScroll(float delta) {
    m_left  ->onScroll(delta);
    m_right ->onScroll(delta);
    m_bottom->onScroll(delta);
    m_center->onScroll(delta);
}

void RegionManager::onKey(int key, int action) {
    m_center->onKey(key, action);
}

void RegionManager::serializeLayout(nlohmann::json& out) const {
    out["leftWidth"] = m_layout.leftWidth;
    out["rightWidth"] = m_layout.rightWidth;
    out["bottomHeight"] = m_layout.bottomHeight;
}

bool RegionManager::deserializeLayout(const nlohmann::json& in) {
    if (in.contains("leftWidth")) m_layout.leftWidth = in["leftWidth"];
    if (in.contains("rightWidth")) m_layout.rightWidth = in["rightWidth"];
    if (in.contains("bottomHeight")) m_layout.bottomHeight = in["bottomHeight"];
    recalculateBounds();
    return true;
}

} // namespace orf
