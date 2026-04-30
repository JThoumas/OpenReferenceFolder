#pragma once
#include "framework/renderer/renderer2d.h"
#include "framework/theme/theme_manager.h"
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>

namespace orf {

class CollapsibleRegion;
class CenterWorkArea;
class TopBar;

struct RegionLayout {
    float topBarHeight     = 32.0f;
    float leftWidth        = 260.0f;   // current width (when expanded)
    float rightWidth       = 260.0f;
    float bottomHeight     = 180.0f;
    float minRegionSize    = 120.0f;   // minimum expanded size
    float collapsedSize    = 14.0f;    // skinny bar width/height
};

class RegionManager {
public:
    explicit RegionManager(Font& font);
    ~RegionManager();

    // Called once by Application after construction
    bool init(int viewportW, int viewportH);

    // Called every frame
    void paint(Renderer2D& renderer);

    // Input — Application forwards all GLFW events here
    void onMouseMove   (float x, float y);
    void onMousePress  (float x, float y, int button);
    void onMouseRelease(float x, float y, int button);
    void onScroll      (float delta);
    void onKey         (int key, int action);

    // Called on window resize
    void resize(int w, int h);

    // Accessors — Application uses these to add content into regions
    CollapsibleRegion* leftRegion()   const { return m_left.get();   }
    CollapsibleRegion* rightRegion()  const { return m_right.get();  }
    CollapsibleRegion* bottomRegion() const { return m_bottom.get(); }
    CenterWorkArea*    center()       const { return m_center.get(); }
    TopBar*            topBar()       const { return m_topBar.get(); }

    // Persistence
    void serializeLayout(nlohmann::json& out) const;
    bool deserializeLayout(const nlohmann::json& in);

    RegionLayout& layout() { return m_layout; }

private:
    void recalculateBounds(); // recomputes all region Rects from m_layout

    // Resize grip hit testing between regions
    bool gripContains(float x, float y, bool horizontal) const;

    Font&         m_font;
    RegionLayout  m_layout;
    int           m_vpWidth  = 0;
    int           m_vpHeight = 0;

    std::unique_ptr<TopBar>             m_topBar;
    std::unique_ptr<CollapsibleRegion>  m_left;
    std::unique_ptr<CollapsibleRegion>  m_right;
    std::unique_ptr<CollapsibleRegion>  m_bottom;
    std::unique_ptr<CenterWorkArea>     m_center;

    // Resize grip drag state
    enum class GripTarget { None, Left, Right, Bottom };
    GripTarget m_activeGrip     = GripTarget::None;
    float      m_gripDragStart  = 0.0f;
    float      m_gripSizeAtDrag = 0.0f;
};

} // namespace orf
