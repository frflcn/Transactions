#pragma once
#include <string>
#include <vector>
#include <fstream>

#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;


namespace Config{

    struct Account{
        string name;
        string access_token;
        string item_id;
        string cursor;

    };

    struct Config{
        string client_id;
        string production_secret;
        string transaction_file;
        vector<Account> accounts;
    };

    void from_json(const json& j, Config& c);

    
    inline Config config;

    void read_config();
    void write_config();

}

