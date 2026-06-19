#include "Config.hpp"
#include <nlohmann/json.hpp>
#include <QDir>
#include <QCoreApplication>

using namespace nlohmann;

namespace Config{

    fstream config_file;

    void from_json(const json& j, Account& a){
        a.name = j["name"];
        a.access_token = j["access_token"];
        a.cursor = j["cursor"];
        a.item_id = j["item_id"];
    }

    void from_json(const json& j, Config& c){
        c.client_id = j["client_id"];
        c.production_secret = j["production_secret"];
        c.transaction_file = j["transaction_file"];
        c.accounts = j["accounts"];
    }

    void to_json(json& j, const Account& a) {
        j = json{{"name", a.name}, {"access_token", a.access_token}, {"cursor", a.cursor}, {"item_id", a.item_id}};
    }

    void to_json(json& j, const Config& c) {
        j = json{{"client_id", c.client_id}, {"production_secret", c.production_secret}, 
                    {"transaction_file", c.transaction_file}, {"accounts", c.accounts}};
    }

    void read_config(){
        string config_filepath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("config.json").toStdString();
        config_file = fstream(config_filepath);
        json jconfig;
        config_file >> jconfig;
        config = jconfig;
    }

    void write_config(){
        config_file.seekp(0);
        json jconfig = config;
        config_file << setw(4) << jconfig;
    }
}