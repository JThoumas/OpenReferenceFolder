#pragma once
#include <string>
#include <vector>
#include <optional>

namespace orf::platform {

// Opens the OS native file picker.
// Returns selected paths, or empty if cancelled.
std::vector<std::string> openFileDialog(
    const std::string& title,
    bool multiSelect = true
);

// Opens the OS native folder picker.
std::optional<std::string> openFolderDialog(
    const std::string& title
);

} // namespace orf::platform
