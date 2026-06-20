#include "PlaidJson.hpp"
#include "LibreOffice.hpp"
#include "PlaidAPI.hpp"
#include "Config.hpp"

#include <QCoreApplication>


#include <iostream>
#include <format>


using namespace std;
using namespace Config;
using namespace PlaidJson;

int main(int argc, char* argv[]){

    cout << "Running..." << endl << endl;

    QCoreApplication app(argc, argv);

    read_config();

    get_spreadsheetdoc();

    for (Config::Account& account : config.accounts){
        
        get_spreadsheet();

        bool has_more = true;
        while(has_more){
            PlaidTransactionsResponse plaidTransResponse = call_transactions(account);
            add_transactions(plaidTransResponse);
            has_more = plaidTransResponse.has_more;
        }
    }

    teardown_libreoffice();

}