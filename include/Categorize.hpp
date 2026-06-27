#pragma once
#include "PlaidJson.hpp"


void compile_hs_categorizer();
void hs_categorize(PlaidJson::Transaction& transaction);
typedef bool (*category_function) (PlaidJson::Transaction, unsigned int, unsigned long long, unsigned long long);

struct Category {
    const char* regex;
    const char* store;
    const char* category;
    const char* subcategory = "";
    const bool checkLessThanAmount = false;
    const double lessThanAmount = 0;
    const bool checkMoreThanAmount = false;
    const double moreThanAmount = 0;
    const category_function func = NULL;
    Category(const char* regex, const char* store, const char* category) 
            : Category(regex, store, category, ""){}

    Category(const char* regex, const char* store, const char* category, const char* subcategory)
            : regex(regex), store(store), category(category), subcategory(subcategory) {}

    Category(const char* regex, const char* store, const char* category, double MoreThanAmount) 
            : regex(regex), store(store), category(category), checkMoreThanAmount(true), moreThanAmount(moreThanAmount) {}

    Category(const char* regex, const char* store, const char* category, bool checkMoreThanAmount, double lessThanAmount) 
            : regex(regex), store(store), category(category), checkLessThanAmount(true), lessThanAmount(lessThanAmount) {}

    Category(const char* regex, const char* store, const char* category, double moreThanAmount, double lessThanAmount) 
            : regex(regex), store(store), category(category), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), checkLessThanAmount(true), lessThanAmount(lessThanAmount) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, double moreThanAmount) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkMoreThanAmount(true), moreThanAmount(moreThanAmount) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, bool checkMoreThanAmount, double lessThanAmount) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkLessThanAmount(true), lessThanAmount(lessThanAmount) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, double moreThanAmount, double lessThanAmount) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), checkLessThanAmount(true), lessThanAmount(lessThanAmount) {}



    Category(const char* regex, const char* store, const char* category, category_function func) 
            : Category(regex, store, category, "", func){}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, category_function func)
            : regex(regex), store(store), category(category), subcategory(subcategory), func(func) {}

    Category(const char* regex, const char* store, const char* category, double MoreThanAmount, category_function func) 
            : regex(regex), store(store), category(category), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), func(func) {}

    Category(const char* regex, const char* store, const char* category, bool checkMoreThanAmount, double lessThanAmount, category_function func) 
            : regex(regex), store(store), category(category), checkLessThanAmount(true), lessThanAmount(lessThanAmount), func(func) {}

    Category(const char* regex, const char* store, const char* category, double moreThanAmount, double lessThanAmount, category_function func) 
            : regex(regex), store(store), category(category), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), checkLessThanAmount(true), lessThanAmount(lessThanAmount), func(func) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, double moreThanAmount, category_function func) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), func(func) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, bool checkMoreThanAmount, double lessThanAmount, category_function func) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkLessThanAmount(true), lessThanAmount(lessThanAmount), func(func) {}

    Category(const char* regex, const char* store, const char* category, const char* subcategory, double moreThanAmount, double lessThanAmount, category_function func) 
            : regex(regex), store(store), category(category), subcategory(subcategory), checkMoreThanAmount(true), moreThanAmount(moreThanAmount), checkLessThanAmount(true), lessThanAmount(lessThanAmount), func(func) {}

};