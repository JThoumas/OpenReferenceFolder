#pragma once
#include "framework/renderer/renderer2d.h"
#include <string>
#include <unordered_map>

namespace orf {

struct Theme {
    // --- Window & Panels ---
    Color windowBackground      = {0.10f, 0.10f, 0.12f, 1.0f};
    Color panelBackground       = {0.14f, 0.14f, 0.17f, 1.0f};
    Color panelBorder           = {0.08f, 0.08f, 0.10f, 1.0f};

    // --- Title Bars & Tab Bars ---
    Color titleBarBackground    = {0.15f, 0.15f, 0.18f, 1.0f};
    Color titleBarText          = {0.80f, 0.80f, 0.80f, 1.0f};
    Color tabBarBackground      = {0.12f, 0.12f, 0.14f, 1.0f};
    Color tabActive             = {0.20f, 0.20f, 0.24f, 1.0f};
    Color tabInactive           = {0.14f, 0.14f, 0.16f, 1.0f};
    Color tabActiveText         = {1.00f, 1.00f, 1.00f, 1.0f};
    Color tabInactiveText       = {0.60f, 0.60f, 0.60f, 1.0f};

    // --- Buttons ---
    Color buttonNormal          = {0.25f, 0.25f, 0.30f, 1.0f};
    Color buttonHover           = {0.35f, 0.35f, 0.42f, 1.0f};
    Color buttonPressed         = {0.18f, 0.18f, 0.22f, 1.0f};
    Color buttonText            = {1.00f, 1.00f, 1.00f, 1.0f};

    // --- Dividers & Scrollbars ---
    Color divider               = {0.10f, 0.10f, 0.12f, 1.0f};
    Color dividerHovered        = {0.35f, 0.35f, 0.40f, 1.0f};
    Color scrollbarTrack        = {0.08f, 0.08f, 0.10f, 0.8f};
    Color scrollbarThumb        = {0.40f, 0.40f, 0.45f, 0.9f};

    // --- File Browser ---
    Color browserBackground     = {0.13f, 0.13f, 0.15f, 1.0f};
    Color browserRowOdd         = {0.00f, 0.00f, 0.00f, 0.08f};
    Color browserSelected       = {0.25f, 0.45f, 0.75f, 1.0f};
    Color browserSelectedText   = {1.00f, 1.00f, 1.00f, 1.0f};
    Color browserText           = {0.85f, 0.85f, 0.85f, 1.0f};
    Color folderIcon            = {0.28f, 0.52f, 0.70f, 0.6f};

    // --- Text ---
    Color textPrimary           = {0.90f, 0.90f, 0.90f, 1.0f};
    Color textSecondary         = {0.60f, 0.60f, 0.60f, 1.0f};
    Color textDisabled          = {0.35f, 0.35f, 0.35f, 1.0f};

    // --- Context Menu ---
    Color contextMenuBackground = {0.18f, 0.18f, 0.21f, 1.0f};
    Color contextMenuHover      = {0.25f, 0.45f, 0.75f, 0.8f};
    Color contextMenuText       = {0.88f, 0.88f, 0.88f, 1.0f};
    Color contextMenuSeparator  = {0.25f, 0.25f, 0.28f, 1.0f};

    // --- Accent ---
    Color accent                = {0.25f, 0.55f, 0.95f, 1.0f};
    Color accentHover           = {0.35f, 0.65f, 1.00f, 1.0f};

    // Theme metadata
    std::string name            = "ORF Dark";
    std::string author          = "";
};

} // namespace orf
