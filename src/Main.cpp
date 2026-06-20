#include "PlaidJson.hpp"
#include "LibreOffice.hpp"
#include "PlaidAPI.hpp"
#include "Config.hpp"
#include "Parser.hpp"

#include <QCoreApplication>
#include <QCommandLineParser>


#include <iostream>
#include <format>


using namespace std;
using namespace Config;
using namespace PlaidJson;

int main(int argc, char* argv[]){

    QCoreApplication app(argc, argv);

    #ifndef NDEBUG
    parser.addOption(startFresh);
    #endif
    parser.addHelpOption();
    parser.process(app);

    cout << "Running..." << endl << endl;

    read_config();

    get_spreadsheetdoc();

    for (Config::Account& account : config.accounts){

        #ifndef NDEBUG
        if (parser.isSet(startFresh)) account.cursor = "";
        #endif
        
        get_spreadsheet(account.name.c_str());

        bool has_more = true;
        while(has_more){
            PlaidTransactionsResponse plaidTransResponse = call_transactions(account);
            add_transactions(plaidTransResponse);
            has_more = plaidTransResponse.has_more;
        }
    }

    teardown_libreoffice();

}