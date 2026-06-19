#include "PlaidJson.hpp"



namespace decimal{
    void from_json(const json& j, Decimal& d){
        if(j.is_null()){
            d = Decimal();
        }
        else{
            double num;
            j.get_to(num);
    
            d = Decimal(std::format("{:.2f}", num));
        }
    }
}

namespace std::chrono{
    void from_json(const json& j, year_month_day& ymd){
        string s;
        j.get_to(s);
        istringstream iss(s);
        int year;
        unsigned int month, day;
        char sep1, sep2;
        iss >> year >> sep1 >> month >> sep2 >> day;
        if (sep1 != '-' || sep2 != '-') {
            throw std::runtime_error("Invalid date format: expected YYYY-MM-DD");
        }
        ymd = year_month_day(std::chrono::year{year}, std::chrono::month{month}, std::chrono::day{day});
    }
}

namespace PlaidJson{
    void from_json(const json& j, Transaction& t){

        j.at("account_id").get_to(t.account_id);

        j.at("amount").get_to(t.amount);
        t.amount = -t.amount;

        j.at("date").get_to(t.date);
        j.at("name").get_to(t.name);

        json jmerchant_name = j.at("merchant_name");
        if (jmerchant_name.is_null()){
            t.merchant_name = "";
        }else{
            j.at("merchant_name").get_to(t.merchant_name);
        }
        
        if (j.contains("original_description")){
            j.at("original_description").get_to(t.original_description);
        }

        j.at("pending").get_to(t.pending);
        j.at("transaction_id").get_to(t.transaction_id);
    }

    void from_json(const json& j, RemovedTransaction& t){
        j.at("account_id").get_to(t.account_id);
        j.at("transaction_id").get_to(t.transaction_id);
    }

    void from_json(const json& j, Balances& b){
        j.at("available").get_to(b.available);
        j.at("current").get_to(b.current);
        j.at("limit").get_to(b.limit);
    }

    void from_json(const json& j, Account& a){
        j.at("account_id").get_to(a.account_id);
        j.at("balances").get_to(a.balances);
        j.at("mask").get_to(a.mask);
        j.at("name").get_to(a.name);
        j.at("official_name").get_to(a.official_name);
        j.at("type").get_to(a.type);
        j.at("subtype").get_to(a.subtype);
    }

    void from_json(const json& j, PlaidTransactionsResponse& response){
        j.at("accounts").get_to(response.accounts);
        j.at("added").get_to(response.added);
        j.at("modified").get_to(response.modified);
        j.at("removed").get_to(response.removed);
        j.at("has_more").get_to(response.has_more);
        j.at("next_cursor").get_to(response.next_cursor);
        j.at("request_id").get_to(response.request_id);
        j.at("transactions_update_status").get_to(response.transactions_update_status);
    }

    ostream& operator<<(ostream& os, const Transaction& t) {
        os << "Merchant: ";
        if (t.merchant_name == ""){
            os << t.name;
        } else{
            os << t.merchant_name;
        }
        os << "\n"
            << "amount: "     << t.amount     << "\n"
            << "date: "       << t.date       << "\n"
            << "Pending: "    << t.pending    << "\n";
        return os;
    }

    bool is_transaction_earlier(const Transaction& first, const Transaction& second){
        if (sys_days(first.date) < sys_days(second.date)){
            return true;
        }
        return false;
    }
    bool is_transaction_later(const Transaction& first, const Transaction& second){
        if (sys_days(first.date) < sys_days(second.date)){
            return false;
        }
        return true;
    }
}

