#include "PlaidJson.hpp"
#include "LibreOffice.hpp"
#include "PlaidAPI.hpp"
#include "Config.hpp"
#include "Parser.hpp"
#include "Categorize.hpp"

#include <QCoreApplication>
#include <QCommandLineParser>


#include <iostream>
#include <format>


using namespace std;
using namespace Config;
using namespace PlaidJson;

int main(int argc, char* argv[]){

    QCoreApplication app(argc, argv);

    parser.addOption(categorizeAll);
    #ifndef NDEBUG
    parser.addOption(startFresh);
    #endif
    parser.addHelpOption();
    parser.process(app);

    cout << "Running..." << endl << endl;

    read_config();

    get_spreadsheetdoc();

    compile_hs_categorizer();

    for (Config::Account& account : config.accounts){

        #ifndef NDEBUG
        if (parser.isSet(startFresh)) account.cursor = "";
        #endif
        
        get_spreadsheet(account.name.c_str());

        if (parser.isSet(categorizeAll)){
            categorize_all();
        }

        bool has_more = true;
        while(has_more){

            PlaidTransactionsResponse plaidTransResponse = call_transactions(account);

            for (Transaction& trans : plaidTransResponse.added){
                hs_categorize(trans);
            }

            add_transactions(plaidTransResponse);
            has_more = plaidTransResponse.has_more;
        }
    }

    teardown_libreoffice();

}