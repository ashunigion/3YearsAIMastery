// =============================================================================
// 08_database_sqlite.cpp — SQLite3 database connection in C++
// =============================================================================
//
// SQLite3 is the simplest SQL database — stored in a single file,
// no server needed. Perfect for learning DB connections in C++.
//
// INSTALL SQLITE3 HEADERS:
//   sudo apt install libsqlite3-dev      ← Ubuntu/Debian
//   sudo dnf install sqlite-devel        ← Fedora/RHEL
//
// COMPILE:
//   g++ -std=c++17 -o db 08_database_sqlite.cpp -lsqlite3
// RUN:
//   ./db
//   ls *.db                              ← see the generated database file
//
// For PostgreSQL/MySQL/MariaDB → use libpq, mysqlclient, or the SOCI library
// For a modern ORM-like approach → use SQLiteCpp or ODB
// =============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sqlite3.h>     // from libsqlite3-dev

// =============================================================================
// RAII WRAPPER — ensures sqlite3* is always closed, even if exceptions throw
// =============================================================================
class Database {
public:
    // ── Open (or create) the database file ───────────────────────────────────
    explicit Database(const std::string& path)
    {
        int rc = sqlite3_open(path.c_str(), &db_);
        if (rc != SQLITE_OK) {
            std::string err = sqlite3_errmsg(db_);
            sqlite3_close(db_);
            db_ = nullptr;
            throw std::runtime_error("Cannot open database: " + err);
        }
        std::cout << "[DB] Opened: " << path << "\n";
    }

    // ── Destructor: always close the DB handle ────────────────────────────────
    ~Database()
    {
        if (db_) {
            sqlite3_close(db_);
            std::cout << "[DB] Closed.\n";
        }
    }

    // Prevent copying (a DB handle should have one owner)
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // ── Execute SQL that returns no rows (CREATE, INSERT, UPDATE, DELETE) ─────
    void execute(const std::string& sql)
    {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string err = errMsg;
            sqlite3_free(errMsg);
            throw std::runtime_error("SQL error: " + err + "\nSQL: " + sql);
        }
    }

    // ── Parameterised INSERT (protects against SQL injection) ─────────────────
    // NEVER build SQL by concatenating user strings → use ?  placeholders
    void insertUser(const std::string& name, int age, const std::string& email)
    {
        const char* sql =
            "INSERT INTO users (name, age, email) VALUES (?, ?, ?);";

        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);

        // Bind parameters by position (1-indexed)
        sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_STATIC);
        sqlite3_bind_int (stmt, 2, age);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);   // always finalize to release resources

        if (rc != SQLITE_DONE)
            throw std::runtime_error("Insert failed");
    }

    // ── Query — returns results as vector of rows ─────────────────────────────
    struct UserRow { int id; std::string name; int age; std::string email; };

    std::vector<UserRow> queryUsers(const std::string& nameFilter = "")
    {
        std::vector<UserRow> results;

        std::string sql = "SELECT id, name, age, email FROM users";
        if (!nameFilter.empty())
            sql += " WHERE name LIKE ?";
        sql += " ORDER BY age;";

        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr);

        if (!nameFilter.empty()) {
            std::string pattern = "%" + nameFilter + "%";
            sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            UserRow row;
            row.id    = sqlite3_column_int (stmt, 0);
            row.name  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            row.age   = sqlite3_column_int (stmt, 2);
            row.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            results.push_back(row);
        }

        sqlite3_finalize(stmt);
        return results;
    }

private:
    sqlite3* db_ = nullptr;
};

// =============================================================================
// MAIN
// =============================================================================
int main()
{
    try {
        // Open (creates file if it doesn't exist; ":memory:" = in-RAM DB)
        Database db("users.db");

        // ── Create table ─────────────────────────────────────────────────────
        db.execute(R"(
            CREATE TABLE IF NOT EXISTS users (
                id    INTEGER PRIMARY KEY AUTOINCREMENT,
                name  TEXT NOT NULL,
                age   INTEGER,
                email TEXT UNIQUE
            );
        )");

        // ── Insert rows ───────────────────────────────────────────────────────
        db.insertUser("Alice",   28, "alice@example.com");
        db.insertUser("Bob",     34, "bob@example.com");
        db.insertUser("Charlie", 22, "charlie@example.com");
        db.insertUser("Diana",   31, "diana@example.com");
        std::cout << "[DB] 4 users inserted.\n";

        // ── Query all ──────────────────────────────────────────────────────────
        auto users = db.queryUsers();
        std::cout << "\n--- All users (sorted by age) ---\n";
        for (auto& u : users)
            std::cout << u.id << " | " << u.name
                      << " | age=" << u.age
                      << " | " << u.email << "\n";

        // ── Filtered query ────────────────────────────────────────────────────
        std::cout << "\n--- Users matching 'a' ---\n";
        for (auto& u : db.queryUsers("a"))
            std::cout << u.name << "\n";

        // ── Update ───────────────────────────────────────────────────────────
        db.execute("UPDATE users SET age = 29 WHERE name = 'Alice';");
        std::cout << "\n[DB] Alice updated.\n";

        // ── Delete ───────────────────────────────────────────────────────────
        db.execute("DELETE FROM users WHERE name = 'Bob';");
        std::cout << "[DB] Bob deleted.\n";

        // Final state
        std::cout << "\n--- Final state ---\n";
        for (auto& u : db.queryUsers())
            std::cout << u.id << " | " << u.name << " | age=" << u.age << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

// =============================================================================
// KEY TAKEAWAYS
//   - RAII: wrap sqlite3* in a class → destructor calls sqlite3_close()
//   - ALWAYS use prepared statements with ?  placeholders for user data
//     → prevents SQL injection (OWASP #3)
//   - sqlite3_prepare_v2 → sqlite3_bind_* → sqlite3_step → sqlite3_finalize
//   - ":memory:" as path → in-RAM database (fast, no file, gone on exit)
//   - For production → add transactions: BEGIN; ... COMMIT; for batch inserts
// =============================================================================
