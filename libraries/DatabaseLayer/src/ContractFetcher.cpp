#include "DatabaseLayer/ContractInfo/ContractFetcher.hpp"
#include "DatabaseLayer/Logger/Logger.hpp"

#include <sqlite3.h>
#include <stdexcept>
#include <cstring>

namespace DatabaseLayer {

ContractFetcher::ContractFetcher(const std::string& name_) : _database(nullptr) {
    int result = sqlite3_open(name_.c_str(), &_database);
    if (result != SQLITE_OK) {
        DB_ERROR("Failed to open database: {}", name_);
        if (_database) {
            sqlite3_close(_database);
            _database = nullptr;
        }
        throw std::runtime_error("Failed to open database: " + name_);
    }
    PrintMetaData(name_);
}

ContractFetcher::~ContractFetcher() noexcept {
    if (_database) {
        sqlite3_close(_database);
        _database = nullptr;
    }
}

void ContractFetcher::PrintMetaData(const std::string& name_) {
    DB_INFO("Database opened: {}", name_);
    if (_database) {
        DB_INFO("SQLite version: {}", sqlite3_libversion());
    }
}

void ContractFetcher::Insert(TableWithColumnIndexT& table_) {
    if (!_database) {
        throw std::runtime_error("Database not initialized");
    }

    // Begin transaction
    ExecuteQuery("BEGIN TRANSACTION");
    
    try {
        // Create table if not exists
        ExecuteQuery(R"(
            CREATE TABLE IF NOT EXISTS contracts (
                token INTEGER PRIMARY KEY,
                symbol TEXT,
                name TEXT,
                exchange TEXT,
                instrument_type TEXT,
                option_type TEXT,
                strike_price REAL,
                expiry_date INTEGER,
                lot_size INTEGER
            )
        )");

        // Prepare insert statement
        const char* insertSQL = "INSERT OR REPLACE INTO contracts VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
        sqlite3_stmt* stmt = nullptr;
        
        int result = sqlite3_prepare_v2(_database, insertSQL, -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare insert statement");
        }

        // Insert data
        for (const auto& row : table_) {
            if (row.size() >= 9) {
                // Bind parameters
                for (size_t i = 0; i < 9 && i < row.size(); ++i) {
                    sqlite3_bind_text(stmt, static_cast<int>(i + 1), row[i].c_str(), -1, SQLITE_STATIC);
                }
                
                // Execute
                result = sqlite3_step(stmt);
                if (result != SQLITE_DONE) {
                    sqlite3_finalize(stmt);
                    throw std::runtime_error("Failed to insert row");
                }
                
                // Reset for next iteration
                sqlite3_reset(stmt);
            }
        }
        
        sqlite3_finalize(stmt);
        ExecuteQuery("COMMIT");
        
    } catch (...) {
        ExecuteQuery("ROLLBACK");
        throw;
    }
}

TableWithColumnNameT ContractFetcher::GetResultWithColumnName(const std::string& query_) {
    if (!_database) {
        throw std::runtime_error("Database not initialized");
    }

    TableWithColumnNameT table;
    sqlite3_stmt* stmt = nullptr;
    
    int result = sqlite3_prepare_v2(_database, query_.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        DB_ERROR("Failed to prepare query: {}", sqlite3_errmsg(_database));
        return table;
    }
    
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        RowWithColumnNameT row;
        int columnCount = sqlite3_column_count(stmt);
        
        for (int i = 0; i < columnCount; i++) {
            const char* columnName = sqlite3_column_name(stmt, i);
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            
            if (columnName && columnValue) {
                row[columnName] = columnValue;
            } else if (columnName) {
                row[columnName] = "";
            }
        }
        table.push_back(std::move(row));
    }
    
    sqlite3_finalize(stmt);
    return table;
}

TableWithColumnIndexT ContractFetcher::GetResultWithColumnIndex(const std::string& query_) {
    if (!_database) {
        throw std::runtime_error("Database not initialized");
    }

    TableWithColumnIndexT table;
    sqlite3_stmt* stmt = nullptr;
    
    int result = sqlite3_prepare_v2(_database, query_.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        DB_ERROR("Failed to prepare query: {}", sqlite3_errmsg(_database));
        return table;
    }
    
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        RowWithColumnIndexT row;
        int columnCount = sqlite3_column_count(stmt);
        
        for (int i = 0; i < columnCount; i++) {
            const char* columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            row.emplace_back(columnValue ? columnValue : "");
        }
        table.push_back(std::move(row));
    }
    
    sqlite3_finalize(stmt);
    return table;
}

void ContractFetcher::ExecuteQuery(const std::string& query_) {
    if (!_database) {
        throw std::runtime_error("Database not initialized");
    }

    char* errorMessage = nullptr;
    int result = sqlite3_exec(_database, query_.c_str(), nullptr, nullptr, &errorMessage);
    
    if (result != SQLITE_OK) {
        std::string error = errorMessage ? errorMessage : "Unknown error";
        if (errorMessage) {
            sqlite3_free(errorMessage);
        }
        DB_ERROR("Query execution failed: {}", error);
        throw std::runtime_error("Query execution failed: " + error);
    }
}

} // namespace DatabaseLayer