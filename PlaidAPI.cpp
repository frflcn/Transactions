#include "PlaidJson.hpp"

#include <fstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
using namespace cpr;
using namespace std;
using namespace nlohmann;

string clientID;
string productionSecret;

void plaid_initialize(){
    fstream file("/home/trevor/MEGA/Finances/Code/Transactions/plaid.json");
    json j;

    file >> j;

    j.at("client_id").get_to(clientID);

    j.at("production_secret").get_to(productionSecret);
   
}

PlaidTransactionsResponse call_transactions(){

    fstream file("/home/trevor/MEGA/Finances/Code/Transactions/fulton-plaid.json");
    json jsend;
    file >> jsend;

    string cursor;
    string accessToken;
    jsend.at("cursor").get_to(cursor);
    jsend.at("access_token").get_to(accessToken);
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
)", clientID, productionSecret, accessToken, cursor)));


    file.seekp(0);
    
    if (r.status_code != 200){
        jsend["cursor"] = "";
        file << setw(4) << jsend;
        throw runtime_error(std::format("Error: Invalid Response From Plaid \nStatus Code: {}\n{}", r.status_code, r.text));
    }

    json jresponse = json::parse(r.text);


    response = jresponse;
    jsend["cursor"] = response.next_cursor;

    file << setw(4) << jsend;
    file.close();
    if (response.accounts.size() > 0 ){
        cout << "Available: "<< response.accounts[0].balances.available << endl <<  "Current: " <<response.accounts[0].balances.current << endl << endl;
    }


    return response;
}