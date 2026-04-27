#pragma once
#include "framework/widgets/widget.h"
#include "framework/widgets/button.h"
#include "framework/widgets/panel.h"
#include "framework/widgets/label.h"
#include "framework/theme/theme_manager.h"
#include "app/model/database.h"
#include "framework/renderer/font.h"
#include <vector>
#include <string>
#include <filesystem>
#include <functional>

namespace orf {

// Tab indices within the Display Center
enum class DisplayCenterTab {
    Workspaces  = 0,
    Themes      = 1,
    Monitors    = 2,
};

class DisplayCenterWidget : public Widget {
public:
    DisplayCenterWidget(Font&         font,
                        ThemeManager& themes,
                        Database&     db);

    using WorkspaceAction = std::function<void(const std::string&)>;
    void setOnSaveWorkspace (WorkspaceAction cb) { m_onSaveWorkspace  = std::move(cb); }
    void setOnApplyWorkspace(WorkspaceAction cb) { m_onApplyWorkspace = std::move(cb); }
    void setOnDeleteWorkspace(WorkspaceAction cb) { m_onDeleteWorkspace = std::move(cb); }

    void onPaint       (Renderer2D& renderer) override;
    void onMousePress  (MouseEvent& e)         override;
    void onMouseMove   (MouseEvent& e)         override;
    void onResize      (int w, int h)          override;

private:
    void paintTabBar   (Renderer2D& renderer);
    void paintWorkspaces(Renderer2D& renderer);
    void paintThemes   (Renderer2D& renderer);
    void paintMonitors (Renderer2D& renderer);

    void loadWorkspaceList();
    void saveCurrentWorkspace(const std::string& name);
    void applyWorkspace(const std::string& name);
    void deleteWorkspace(const std::string& name);

    Font&         m_font;
    ThemeManager& m_themes;
    Database&     m_db;

    DisplayCenterTab          m_activeTab = DisplayCenterTab::Workspaces;
    std::vector<std::string>  m_workspaceNames;
    std::vector<std::string>  m_themeFiles;
    int                       m_hoveredItem    = -1;
    int                       m_selectedLayout = -1;

    WorkspaceAction m_onSaveWorkspace;
    WorkspaceAction m_onApplyWorkspace;
    WorkspaceAction m_onDeleteWorkspace;
};

} // namespace orf
