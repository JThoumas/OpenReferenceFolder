#include "database.h"
#include "framework/core/log.h"
#include <sqlite3.h>

namespace orf {

Database::~Database() { close(); }

void Database::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool Database::open(const std::string& path) {
    int rc = sqlite3_open(path.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Database: failed to open " << path
                  << " — " << sqlite3_errmsg(m_db));
        m_db = nullptr;
        return false;
    }
    // WAL mode — better concurrent read performance
    execSQL("PRAGMA journal_mode=WAL;");
    execSQL("PRAGMA foreign_keys=ON;");
    return applySchema();
}

bool Database::applySchema() {
    return execSQL(R"(
        CREATE TABLE IF NOT EXISTS nodes (
            id           INTEGER PRIMARY KEY AUTOINCREMENT,
            parent_id    INTEGER REFERENCES nodes(id) ON DELETE CASCADE,
            node_type    INTEGER NOT NULL DEFAULT 0,
            display_name TEXT    NOT NULL,
            real_path    TEXT    NOT NULL DEFAULT ''
        );
    )");
}

bool Database::execSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Database SQL error: " << errMsg);
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::vector<ORFNode> Database::loadTree() {
    return loadChildren(-1);
}

std::vector<ORFNode> Database::loadChildren(int64_t parentId) {
    std::vector<ORFNode> result;

    const char* sql = parentId == -1
        ? "SELECT id, parent_id, node_type, display_name, real_path "
          "FROM nodes WHERE parent_id IS NULL ORDER BY display_name;"
        : "SELECT id, parent_id, node_type, display_name, real_path "
          "FROM nodes WHERE parent_id = ? ORDER BY display_name;";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (parentId != -1)
        sqlite3_bind_int64(stmt, 1, parentId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ORFNode node;
        node.id          = sqlite3_column_int64(stmt, 0);
        // parent_id may be NULL for root nodes
        node.parentId    = (sqlite3_column_type(stmt, 1) == SQLITE_NULL)
                           ? -1
                           : sqlite3_column_int64(stmt, 1);
        node.type        = static_cast<NodeType>(sqlite3_column_int(stmt, 2));
        node.displayName = reinterpret_cast<const char*>(
                               sqlite3_column_text(stmt, 3));
        node.realPath    = reinterpret_cast<const char*>(
                               sqlite3_column_text(stmt, 4));
        node.children    = loadChildren(node.id);
        result.push_back(std::move(node));
    }

    sqlite3_finalize(stmt);
    return result;
}

bool Database::saveNode(ORFNode& node) {
    if (node.id == -1) {
        // INSERT
        const char* sql =
            "INSERT INTO nodes (parent_id, node_type, display_name, real_path) "
            "VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
        if (node.parentId == -1)
            sqlite3_bind_null(stmt, 1);
        else
            sqlite3_bind_int64(stmt, 1, node.parentId);
        sqlite3_bind_int  (stmt, 2, static_cast<int>(node.type));
        sqlite3_bind_text (stmt, 3, node.displayName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text (stmt, 4, node.realPath.string().c_str(), -1, SQLITE_TRANSIENT);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            LOG_ERROR("Database: INSERT failed — " << sqlite3_errmsg(m_db));
            return false;
        }
        node.id = sqlite3_last_insert_rowid(m_db);
    } else {
        // UPDATE
        const char* sql =
            "UPDATE nodes SET parent_id=?, node_type=?, display_name=?, "
            "real_path=? WHERE id=?;";
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
        if (node.parentId == -1)
            sqlite3_bind_null(stmt, 1);
        else
            sqlite3_bind_int64(stmt, 1, node.parentId);
        sqlite3_bind_int  (stmt, 2, static_cast<int>(node.type));
        sqlite3_bind_text (stmt, 3, node.displayName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text (stmt, 4, node.realPath.string().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 5, node.id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            LOG_ERROR("Database: UPDATE failed — " << sqlite3_errmsg(m_db));
            return false;
        }
    }
    return true;
}

bool Database::deleteNode(int64_t id) {
    // CASCADE handles children automatically (foreign_keys=ON)
    const char* sql = "DELETE FROM nodes WHERE id=?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool Database::moveNode(int64_t id, int64_t newParentId) {
    const char* sql = "UPDATE nodes SET parent_id=? WHERE id=?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (newParentId == -1)
        sqlite3_bind_null(stmt, 1);
    else
        sqlite3_bind_int64(stmt, 1, newParentId);
    sqlite3_bind_int64(stmt, 2, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool Database::renameNode(int64_t id, const std::string& newName) {
    const char* sql = "UPDATE nodes SET display_name=? WHERE id=?;";
    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text (stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 2, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

} // namespace orf
