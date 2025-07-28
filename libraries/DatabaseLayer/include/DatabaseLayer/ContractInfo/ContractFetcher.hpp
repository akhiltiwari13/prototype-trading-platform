#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct sqlite3;

namespace DatabaseLayer {
    using RowWithColumnNameT    = std::unordered_map<std::string, std::string>;
    using RowWithColumnIndexT   = std::vector<std::string>;
    using TableWithColumnNameT  = std::vector<RowWithColumnNameT>;
    using TableWithColumnIndexT = std::vector<RowWithColumnIndexT>;

    /**
     * @brief Database contract fetcher for trading instruments
     */
    class ContractFetcher final {
      public:
        explicit ContractFetcher(const std::string& name_);
        ~ContractFetcher() noexcept;

        /**
         * @brief Insert data into database
         */
        void Insert(TableWithColumnIndexT& table_);

        /**
         * @brief Execute a database query
         */
        void ExecuteQuery(const std::string& query_);

        /**
         * @brief Get query results with column names
         */
        auto GetResultWithColumnName(const std::string& query_) -> TableWithColumnNameT;

        /**
         * @brief Get query results with column indices
         */
        auto GetResultWithColumnIndex(const std::string& query_) -> TableWithColumnIndexT;

      private:
        void PrintMetaData(const std::string& name_);
        sqlite3* _database;
    };

} // namespace DatabaseLayer
