#pragma once

#include "PlaidJson.hpp"


void add_transactions(const PlaidJson::PlaidTransactionsResponse response);
void get_spreadsheetdoc();
void get_spreadsheet();
void teardown_libreoffice();
