#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>

namespace orf {

enum class NodeType {
    VirtualFolder,   // Exists only in ORF — no real filesystem path
    RealFolder,      // Points to a real directory
    File             // Points to a real file
};

struct ORFNode {
    int64_t     id          = -1;     // SQLite rowid, -1 = not yet persisted
    int64_t     parentId    = -1;     // -1 = root level
    NodeType    type        = NodeType::VirtualFolder;
    std::string displayName;          // Name shown in the UI
    std::filesystem::path realPath;   // Empty for VirtualFolder

    std::vector<ORFNode> children;    // Populated when expanded

    bool isExpanded = false;
    bool isSelected = false;

    // Convenience
    bool isFolder() const {
        return type == NodeType::VirtualFolder
            || type == NodeType::RealFolder;
    }

    bool hasRealPath() const {
        return !realPath.empty();
    }
};

} // namespace orf
