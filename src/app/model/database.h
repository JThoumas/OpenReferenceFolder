#pragma once
#include "orf_node.h"
#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>

namespace orf {

// Owns the SQLite connection and all persistence for ORFNodes.
// All methods are synchronous — call from the main thread only.
class Database {
public:
    Database()  = default;
    ~Database();

    Database(const Database&)            = delete;
    Database& operator=(const Database&) = delete;

    // Open (or create) the database at the given path.
    // Returns false if the file can't be opened or schema migration fails.
    bool open(const std::string& path);
    void close();

    bool isOpen() const { return m_db != nullptr; }

    // Load the full node tree from the database.
    // Returns root-level nodes; children are populated recursively.
    std::vector<ORFNode> loadTree();

    // Persist a node. If node.id == -1, inserts and sets the id.
    // If node.id >= 0, updates the existing row.
    bool saveNode(ORFNode& node);

    // Delete a node and all its descendants.
    bool deleteNode(int64_t id);

    // Move a node to a new parent.
    bool moveNode(int64_t id, int64_t newParentId);

    // Rename a node's display name.
    bool renameNode(int64_t id, const std::string& newName);

private:
    bool applySchema();
    bool execSQL(const std::string& sql);

    std::vector<ORFNode> loadChildren(int64_t parentId);

    sqlite3* m_db = nullptr;
};

} // namespace orf
