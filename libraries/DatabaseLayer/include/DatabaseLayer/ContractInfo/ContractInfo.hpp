#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace DatabaseLayer {
    enum Exchange : uint8_t;
    enum Instrument : uint8_t;
    enum OptionType : uint8_t;

    struct ResultSetT;
    using ResultSetPtrT             = const ResultSetT*;
    using RowWithColumnNameT        = std::unordered_map<std::string, std::string>;
    using RowWithColumnIndexT       = std::vector<std::string>;
    using TableWithColumnNameT      = std::vector<RowWithColumnNameT>;
    using TableWithColumnIndexT     = std::vector<RowWithColumnIndexT>;
    using ResultSetLoadingCallbackT = std::function<void(ResultSetPtrT, float, float, float)>;

    /**
     * @brief Contract information manager for trading instruments
     */
    class ContractInfo {
      public:
        static void Initialize(const std::string& name_, const ResultSetLoadingCallbackT& callback_);
        static auto GetResultSet(uint32_t token_) -> ResultSetPtrT;

        // Token and basic info
        static auto GetToken(const std::string& name_) -> uint32_t;
        static auto GetExpiryDate(uint32_t token_) -> uint32_t;
        static auto GetLotMultiple(uint32_t token_) -> uint32_t;
        static auto GetLotSize(uint32_t token_) -> uint32_t;
        static auto GetTickSize(uint32_t token_) -> uint32_t;
        static auto GetDivisor(uint32_t token_) -> uint32_t;
        static auto GetFuture(uint32_t token_) -> uint32_t;
        static auto GetOppositeToken(uint32_t token_) -> uint32_t;
        static auto GetStrikePrice(uint32_t token_) -> float;

        // Enum getters
        static auto GetInstType(uint32_t token_) -> Instrument;
        static auto GetOption(uint32_t token_) -> OptionType;
        static auto GetExchange(uint32_t token_) -> Exchange;

        // String getters
        static auto GetSymbol(uint32_t token_) -> std::string;
        static auto GetSegment(uint32_t token_) -> std::string;
        static auto GetName(uint32_t token_) -> std::string;
        static auto GetDescription(uint32_t token_) -> std::string;

        // Type checkers
        static auto IsOption(uint32_t token_) -> bool;
        static auto IsEquity(uint32_t token_) -> bool;
        static auto IsCall(uint32_t token_) -> bool;
        static auto IsPut(uint32_t token_) -> bool;
        static auto IsFuture(uint32_t token_) -> bool;

        // Database operations
        static void ExecuteQuery(const std::string& query_);
        static auto GetResultWithIndex(const std::string& query_) -> TableWithColumnIndexT;
        static auto GetResultWithName(const std::string& query_) -> TableWithColumnNameT;

        // Enum converters
        static auto GetOptionType(const std::string& option_) -> OptionType;
        static auto GetInstrumentType(const std::string& type_) -> Instrument;
        static auto GetExchange(const std::string& exchange_) -> Exchange;
    };

} // namespace DatabaseLayer