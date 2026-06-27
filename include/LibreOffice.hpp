#pragma once

#include "PlaidJson.hpp"

#include <com/sun/star/frame/XComponentLoader.hpp>


void categorize_all();
void add_transactions(const PlaidJson::PlaidTransactionsResponse response);
void get_spreadsheetdoc();
void get_spreadsheet(const char* accountName);
void teardown_libreoffice();

inline css::uno::Reference<css::frame::XComponentLoader> componentLoader;