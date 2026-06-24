#pragma once

#include "PlaidJson.hpp"

void categorize_all();
void add_transactions(const PlaidJson::PlaidTransactionsResponse response);
void get_spreadsheetdoc();
void get_spreadsheet(const char* accountName);
void teardown_libreoffice();
