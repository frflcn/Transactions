#include "PlaidJson.hpp"
#include "LibreOffice.hpp"
#include "PlaidAPI.hpp"




#include <iostream>
#include <format>





using namespace std;

int main(int argc, char* argv[]){

    cout << "Running..." << endl << endl;

    plaid_initialize();
    bool has_more = true;



    while(has_more){
        PlaidTransactionsResponse plaidTransResponse = call_transactions();
        add_transactions(plaidTransResponse);
        has_more = plaidTransResponse.has_more;
    }





}