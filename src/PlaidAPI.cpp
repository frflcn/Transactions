#include "PlaidJson.hpp"
#include "Config.hpp"

#include <fstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>


using namespace cpr;
using namespace std;
using namespace nlohmann;

using namespace Config;
using namespace PlaidJson;


string clientID;
string productionSecret;


PlaidTransactionsResponse call_transactions(Config::Account& account){

    PlaidTransactionsResponse response;

    Response r = Post(Url("https://production.plaid.com/transactions/sync"),
    Header{{"Content-Type", "application/json"}, {"Accept", "application/json"}},
    Body(std::format(R"(
{{
    "client_id": "{}",
    "secret": "{}",
    "access_token": "{}",
    "cursor" : "{}",
    "options": {{
        "include_original_description" : true
    }}
}}
)", config.client_id, config.production_secret, account.access_token, account.cursor)));


    
    if (r.status_code != 200){
        throw runtime_error(std::format("Error: Invalid Response From Plaid \nStatus Code: {}\n{}", r.status_code, r.text));
    }

    json jresponse = json::parse(r.text);


    response = jresponse;

    account.cursor = response.next_cursor;
    write_config();

    if (response.accounts.size() > 0 ){
        cout << "Available: " << response.accounts[0].balances.available << endl <<  "Current: " << response.accounts[0].balances.current << endl << endl;
    }


    return response;
}