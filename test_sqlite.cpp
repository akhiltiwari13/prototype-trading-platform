#include <sqlite3.h>
#include <iostream>
#include <string>

int main() {
    sqlite3* db;
    int result = sqlite3_open(":memory:", &db);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    std::cout << "SQLite version: " << sqlite3_libversion() << std::endl;
    std::cout << "Database opened successfully!" << std::endl;
    
    // Create a simple table
    const char* createTable = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT)";
    char* errorMessage = nullptr;
    result = sqlite3_exec(db, createTable, nullptr, nullptr, &errorMessage);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return 1;
    }
    
    std::cout << "Table created successfully!" << std::endl;
    
    sqlite3_close(db);
    return 0;
}