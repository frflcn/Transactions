#include "Categorize.hpp"
#include "Categories.hpp"
#include "PlaidJson.hpp"

#include <string>
#include <hs/hs.h>
#include <vector>

using namespace std;

using namespace PlaidJson;


const unsigned int flags[] = {};
const unsigned int ids[] = {};

hs_database_t* db;
hs_scratch_t* scratch;


void compile_hs_categorizer(){
    vector<const char*> vec_regex;
    vector<unsigned int> vec_flags;
    vector<unsigned int> vec_ids;
    unsigned int id = 0;
    for(const Category& cat : categories){
        vec_regex.emplace_back(cat.regex);
        vec_ids.emplace_back(id++);
        vec_flags.emplace_back(HS_FLAG_SINGLEMATCH);
    }
    hs_compile_error_t* comp_err;
    hs_error_t err = hs_compile_multi(vec_regex.data(), vec_flags.data(), vec_ids.data(), vec_regex.size(), HS_MODE_BLOCK, NULL, &db, &comp_err);
    if (err != HS_SUCCESS){
        cerr << "Hyperscan Compile Error: " << endl;
        cerr << "ID: " << comp_err->expression << endl;
        cerr << comp_err->message << endl << endl;
    }
    hs_alloc_scratch(db, &scratch);
}

int call_back(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void* context){
    Transaction* trans = (Transaction*)context;
    Category category = categories[id];


    if (category.checkLessThanAmount && stod(trans->amount.to_sci()) >= category.lessThanAmount){
        return 0;
    }
    if (category.checkMoreThanAmount && stod(trans->amount.to_sci()) <= category.moreThanAmount){
        return 0;
    }

    trans->store = category.store;
    trans->subcategory = category.subcategory;
    trans->category = category.category;

    return 1;
}


void hs_categorize(Transaction& transaction){
    hs_scan(db, transaction.original_description.c_str(), transaction.original_description.length(), 0, scratch, call_back, &transaction);
}

