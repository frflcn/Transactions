#pragma once

#include "nlohmann/json.hpp"
#include "decimal.hh"
#include <string>
#include <chrono>
#include <vector>

using namespace std;
using decimal::Decimal;
using namespace std::chrono;
using json = nlohmann::json;


namespace PlaidJson {
        struct RemovedTransaction{
        string account_id;
        string transaction_id;

    };
    struct Transaction{
        string account_id;
        Decimal amount;
        std::chrono::year_month_day date;
        string name;
        string merchant_name;
        bool pending;
        string transaction_id;
        string original_description;
        friend ostream& operator<<(ostream& os, const Transaction& t);

    };

    struct Balances{
        Decimal available;
        Decimal current;
        Decimal limit;
    };

    struct Account{
        string account_id;
        Balances balances;
        string mask;
        string name;
        string official_name;
        string subtype;
        string type;
    };


    struct PlaidTransactionsResponse{

        vector<Account> accounts;
        vector<Transaction> added;
        vector<Transaction> modified;
        vector<RemovedTransaction> removed;
        bool has_more;
        string next_cursor;
        string request_id;
        string transactions_update_status;
    };

    void from_json(const json& j, PlaidTransactionsResponse& response);
    bool is_transaction_earlier(const Transaction& first, const Transaction& second);
    bool is_transaction_later(const Transaction& first, const Transaction& second);
}


