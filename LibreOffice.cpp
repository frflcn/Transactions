#include "PlaidJson.hpp"


#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/sheet/XSpreadsheetDocument.hpp>
#include <com/sun/star/sheet/XSpreadsheet.hpp>
#include <com/sun/star/table/CellRangeAddress.hpp>
#include <com/sun/star/sheet/XCellRangeAddressable.hpp>
#include <com/sun/star/sheet/XCellRangeData.hpp>
#include <com/sun/star/table/XColumnRowRange.hpp>
#include <com/sun/star/table/XTableRows.hpp>
#include <com/sun/star/sheet/XUsedAreaCursor.hpp>
#include <com/sun/star/util/XNumberFormatsSupplier.hpp>
#include <com/sun/star/beans/XFastPropertySet.hpp>
#include <com/sun/star/sheet/XCellSeries.hpp>
using namespace css;
using namespace css;
using namespace uno;
using namespace lang;
using namespace frame;
using namespace sheet;
using namespace beans;
using namespace table;
using namespace util;
using namespace rtl;

static const int DATE_COLUMN                    = 0;
static const int NAME_COLUMN                    = 1;
static const int MERCHANT_NAME_COLUMN           = 2;
static const int AMOUNT_COLUMN                  = 3;
static const int ACCOUNT_BALANCE_COLUMN         = 4;
static const int PENDING_COLUMN                 = 5;
static const int TRANSACTION_ID_COLUMN          = 6;
static const int ORIGINAL_DESCRIPTION_COLUMN    = 7;
static const int LAST_COLUMN_INDEX              = 7;

static const int START_ROW                      = 1;

void set_row(Sequence<Any>& row, const Transaction trans );
void modify_rows(Reference<XSpreadsheet> xSpreadsheet, vector<Transaction> transactions);
void delete_rows(Reference<XSpreadsheet> xSpreadsheet, const vector<RemovedTransaction> transactions);
void insert_rows(Reference<XSpreadsheet> xSpreadsheet, vector<Transaction> transactions);
void format(Reference<XSpreadsheetDocument> xSpreadsheetDocument, const Reference<XSpreadsheet> xSpreadsheet);
void add_balance(Reference<XSpreadsheet> xSpreadsheet, Account account);
string cell_address_from_index(int row, int col);
Reference<XSpreadsheet> get_spreadsheet();

void add_transactions(PlaidTransactionsResponse response){

        Reference<XSpreadsheet> xSpreadsheet = get_spreadsheet();

        modify_rows(xSpreadsheet, response.modified);


        delete_rows(xSpreadsheet, response.removed);
        insert_rows(xSpreadsheet, response.added);

        if (response.accounts.size() > 0){
            add_balance(xSpreadsheet, response.accounts[0]);
        }



        //format(xSpreadsheetDoc, xSpreadsheet);



}

Reference<XSpreadsheet> get_spreadsheet(){
        Reference<XComponentContext> xContext(cppu::bootstrap());
        Reference<css::lang::XMultiComponentFactory> xMCF = xContext->getServiceManager();
        Reference<XInterface> oDesktop = xMCF->createInstanceWithContext(
            "com.sun.star.frame.Desktop", xContext);
        Reference<XComponentLoader> xComponentLoader
            = Reference<XComponentLoader>(oDesktop, UNO_QUERY_THROW);

        Sequence<PropertyValue> loadProps(1);


        loadProps[0].Name = OUString::createFromAscii("Hidden");
        loadProps[0].Value <<= true;
        Reference<XComponent> xSpreadsheetComponent = xComponentLoader->loadComponentFromURL(
            "file:///home/trevor/MEGA/Finances/Transactions.ods", "_default", 0, loadProps);
        Reference<XSpreadsheetDocument> xSpreadsheetDoc(xSpreadsheetComponent, UNO_QUERY_THROW);
        Any sheet = xSpreadsheetDoc->getSheets()->getByName(OUString::createFromAscii("Fulton"));


        return Reference<XSpreadsheet>(sheet, UNO_QUERY_THROW);
}

string cell_address_from_index(int row, int col){
    char first_letter = col / 26;
    char second_letter = col % 26;
    first_letter += 64;
    second_letter += 65;
    if (first_letter == 64){
        return std::format("{}{}", second_letter, row + 1);
    } else {
        return std::format("{}{}{}", first_letter, second_letter, row + 1);
    }
}

void add_balance(Reference<XSpreadsheet> xSpreadsheet, Account account){

    Reference<XCell> xCell = xSpreadsheet->getCellByPosition(ACCOUNT_BALANCE_COLUMN, START_ROW);
    OUString formula = OUString::createFromAscii(std::format("={} - {}", cell_address_from_index(START_ROW - 1, ACCOUNT_BALANCE_COLUMN), cell_address_from_index(START_ROW - 1, AMOUNT_COLUMN)).c_str());
    xCell->setFormula(formula);
    Reference<XSheetCellCursor> xSheetCellCursor = xSpreadsheet->createCursor();
    Reference<XUsedAreaCursor> xUsedCursor(xSheetCellCursor, UNO_QUERY_THROW);
    xUsedCursor->gotoEndOfUsedArea(true);
    Reference<XCellRangeAddressable> xAddr(xUsedCursor, UNO_QUERY_THROW);
    CellRangeAddress addr = xAddr->getRangeAddress();
    Reference<XCellRange> xCellRange = xSpreadsheet->getCellRangeByPosition(ACCOUNT_BALANCE_COLUMN, START_ROW, ACCOUNT_BALANCE_COLUMN, addr.EndRow);
    Reference<XSheetCellRange> xSheetCellRange(xCellRange, UNO_QUERY_THROW);
    Reference<XCellSeries> xCellSeries(xSheetCellRange, UNO_QUERY_THROW);
    xCellSeries->fillAuto(FillDirection_TO_BOTTOM, 1);
    xCell->setValue(std::stod(account.balances.available.to_eng()));
}

void modify_rows(Reference<XSpreadsheet> xSpreadsheet, const vector<Transaction> transactions){

    vector<RemovedTransaction> removedTransactions = vector<RemovedTransaction>();
    for (int i = 0; i < transactions.size(); ++i){
        removedTransactions.emplace_back(RemovedTransaction{
            .account_id = transactions[i].account_id,
            .transaction_id = transactions[i].transaction_id
        });
    }
    delete_rows(xSpreadsheet, removedTransactions);
    insert_rows(xSpreadsheet, transactions);
}
void delete_rows(Reference<XSpreadsheet> xSpreadsheet, const vector<RemovedTransaction> transactions){
    Reference<XSheetCellCursor> xCursor = xSpreadsheet->createCursor();
    Reference<XUsedAreaCursor> xUsedCursor(xCursor, UNO_QUERY_THROW);

    xUsedCursor->gotoEndOfUsedArea(true);

    // Get the position of the last used cell
    Reference<XCellRangeAddressable> xAddr(xUsedCursor, UNO_QUERY_THROW);
    CellRangeAddress addr = xAddr->getRangeAddress();
    
    Reference<XCellRange> xCellRange = xSpreadsheet->getCellRangeByPosition(0,0,LAST_COLUMN_INDEX,addr.EndRow);
    Reference<XCellRangeData> xCellRangeData(xCellRange, UNO_QUERY_THROW);
    Sequence<Sequence<Any>> xCellData = xCellRangeData->getDataArray();
    vector<int> delete_me_indexes = vector<int>();

    for (int i = 0; i < transactions.size(); i++){
        OUString str;
        
        for (int j = 0; j < xCellData.getLength(); j ++){
            xCellData[j][TRANSACTION_ID_COLUMN] >>= str;
            if (str.equalsAscii(transactions[i].transaction_id.c_str())){
                delete_me_indexes.emplace_back(j);
                break;
            }
        }
    }
    std::sort(delete_me_indexes.begin(), delete_me_indexes.end(), [](int i, int j)
    {if (i > j){
        return true;
    }  
    return false; 
    });
    Reference<XColumnRowRange> xColumnRowRange(xSpreadsheet, UNO_QUERY_THROW);
    Reference<XTableRows> xTableRows = xColumnRowRange->getRows();
    for (int i = 0; i < delete_me_indexes.size(); i++){
        xTableRows->removeByIndex(delete_me_indexes[i],1);
    }

}

void insert_rows(Reference<XSpreadsheet> xSpreadsheet, vector<Transaction> transactions){



    //Sort Transactions by date
    std::sort(transactions.begin(), transactions.end(), is_transaction_later);
 

    //Get Data From Spreadsheet
    Reference<XSheetCellCursor> xCursor = xSpreadsheet->createCursor();
    Reference<XUsedAreaCursor> xUsedCursor(xCursor, UNO_QUERY_THROW);
    xUsedCursor->gotoEndOfUsedArea(true);
    Reference<XCellRangeAddressable> xAddr(xUsedCursor, UNO_QUERY_THROW);
    CellRangeAddress addr = xAddr->getRangeAddress();
    Reference<XCellRange> xCellRange = xSpreadsheet->getCellRangeByPosition(0,0,LAST_COLUMN_INDEX,addr.EndRow);
    Reference<XCellRangeData> xCellRangeData(xCellRange, UNO_QUERY_THROW);
    Sequence<Sequence<Any>> xCellData = xCellRangeData->getDataArray();

    //Find Insertions
    int spreadsheet_index = 0;
    double date;
    int trans_add_date;
    struct index_length{
        int index {};
        int length {1};
    };
    vector<index_length> index_lengths = vector<index_length>();
    for (int trans_add_index = 0; trans_add_index < transactions.size();){
        trans_add_date = (sys_days(transactions[trans_add_index].date) - sys_days(year_month_day(year(1899), month(12), day(30)))).count();
        index_lengths.emplace_back(index_length());
        while(spreadsheet_index < xCellData.getLength()){

            if (!(xCellData[spreadsheet_index][0] >>= date)){
                ++spreadsheet_index;
                continue;
            }
            if (date <= trans_add_date){
                index_lengths[index_lengths.size()-1].index = spreadsheet_index + trans_add_index;
                ++trans_add_index;
                while(trans_add_index < transactions.size()){
                    if(date <= (sys_days(transactions[trans_add_index].date) - sys_days(year_month_day(year(1899), month(12), day(30)))).count() ){
                        index_lengths[index_lengths.size()-1].length += 1;
                        trans_add_index++;
                    }
                    else{
                        break;
                    }
                }
                break;
            }
            ++spreadsheet_index;
        }
        if(spreadsheet_index == xCellData.getLength()){
            // index_length temp_index_length = {.index = spreadsheet_index + trans_add_index, .length = transactions.size() - trans_add_index};
            // index_lengths.emplace_back(temp_index_length);
            index_lengths[index_lengths.size()-1].index = spreadsheet_index + trans_add_index;
            index_lengths[index_lengths.size()-1].length = transactions.size() - trans_add_index;
            break;
        }
    }

    //Insert Empty Rows
    Reference<XColumnRowRange> xColumnRowRange(xSpreadsheet, UNO_QUERY_THROW);
    Reference<XTableRows> xTableRows = xColumnRowRange->getRows();
    for (int i = 0; i < index_lengths.size(); i++){
        xTableRows->insertByIndex(index_lengths[i].index, index_lengths[i].length);
    }

    //Fill Empty Rows
    xCellRange = xSpreadsheet->getCellRangeByPosition(0,0,LAST_COLUMN_INDEX,addr.EndRow + transactions.size());
    xCellRangeData = Reference<XCellRangeData>(xCellRange, UNO_QUERY_THROW);
    xCellData = xCellRangeData->getDataArray();
    int trans_add_index = 0;
    for (int i = 0; i < index_lengths.size(); i++){
        for(int j = 0; j < index_lengths[i].length; j++){
            //xCellData[index_lengths[i].index + j][0] <<= ((sys_days(transactions[trans_add_index++].date) - sys_days(year_month_day(year(1899), month(12), day(30)))).count());
            set_row(xCellData[index_lengths[i].index + j], transactions[trans_add_index++]);
        }
    }
    try{
            xCellRangeData->setDataArray(xCellData);
    }
    catch(RuntimeException e){
        cout << "error: " << e.Message << endl;
    }


}
void format(Reference<XSpreadsheetDocument> xSpreadsheetDocument, const Reference<XSpreadsheet> xSpreadsheet){

    Reference<XNumberFormatsSupplier> xNumberFormatSupplier(xSpreadsheetDocument, UNO_QUERY_THROW);
    Reference<XNumberFormats> xNumberFormats = xNumberFormatSupplier->getNumberFormats();
    lang::Locale locale;
    Sequence<sal_Int32> keys = xNumberFormats->queryKeys(8, locale, false);
    cout << std::format("Length: {}", keys.getLength()) << endl;
    OUString strProp;
    sal_Bool boolProp;
    short shortProp;
    Reference<XPropertySet> xPropertySet;
    // for (int i = 0 ; i <  keys.getLength(); i++){
    //     xPropertySet = xNumberFormats->getByKey(keys[i]);
    //     //Reference<XFastPropertySet> xFastPropertySet(xPropertySet, UNO_QUERY_THROW);

    //     Sequence<Property> properties =  xPropertySet->getPropertySetInfo()->getProperties();
    //     for(int j = 0; j <  properties.getLength(); j++){
    //         Any any = xPropertySet->getPropertyValue(properties[j].Name);
    //         if (any >>= strProp){

    //             cout << std::format("{} : {} : string", OUStringToOString(properties[j].Name, RTL_TEXTENCODING_UTF8).getStr(), OUStringToOString(strProp, RTL_TEXTENCODING_UTF8).getStr()) << endl;

    //         }else if(any >>= shortProp){
    //             cout << properties[j].Name << " : " << shortProp << " : sal_Bool" << endl;
    //         }
    //         else if(any >>= boolProp){
    //             cout << properties[j].Name << " : " << (boolProp ? "true" : "false") << " : sal_Bool" << endl;
    //         } else {
    //             cout << properties[j].Name << ":  " << OUStringToOString(any.getValueTypeName(),RTL_TEXTENCODING_UTF8) << endl;
    //         }
            
    //     }
    //     cout << endl << endl;
    // }
    Reference<XCellRange> xCellRange = xSpreadsheet->getCellRangeByPosition(4,0,4,1000);
     xPropertySet = Reference<XPropertySet>(xCellRange, UNO_QUERY_THROW);
 
        //Reference<XFastPropertySet> xFastPropertySet(xPropertySet, UNO_QUERY_THROW);

    Sequence<Property> properties =  xPropertySet->getPropertySetInfo()->getProperties();
    long longProp;
    for(int j = 0; j <  properties.getLength(); j++){
        Any any = xPropertySet->getPropertyValue(properties[j].Name);
        if (any >>= strProp){

            cout << std::format("{} : {} : string", OUStringToOString(properties[j].Name, RTL_TEXTENCODING_UTF8).getStr(), OUStringToOString(strProp, RTL_TEXTENCODING_UTF8).getStr()) << endl;

        }else if(any >>= shortProp){
            cout << properties[j].Name << " : " << shortProp << " : sal_Bool" << endl;
        }
        else if(any >>= boolProp){
            cout << properties[j].Name << " : " << (boolProp ? "true" : "false") << " : sal_Bool" << endl;
        } else if(any >>= longProp){
            cout << properties[j].Name << " : " << longProp << " : sal_Bool" << endl;
        }else {
            cout << properties[j].Name << ":  " << OUStringToOString(any.getValueTypeName(),RTL_TEXTENCODING_UTF8) << endl;
        }
        
    }
    cout << endl << endl;
    long longNumFormat;
    xPropertySet->getPropertyValue(OUString("NumberFormat")) >>= longNumFormat;
    cout << "hello" <<endl;
    xPropertySet = xNumberFormats->getByKey(longNumFormat);
    properties =  xPropertySet->getPropertySetInfo()->getProperties();
        cout << "hello" <<endl;
    for(int j = 0; j <  properties.getLength(); j++){
        Any any = xPropertySet->getPropertyValue(properties[j].Name);
        if (any >>= strProp){

            cout << std::format("{} : {} : string", OUStringToOString(properties[j].Name, RTL_TEXTENCODING_UTF8).getStr(), OUStringToOString(strProp, RTL_TEXTENCODING_UTF8).getStr()) << endl;

        }else if(any >>= shortProp){
            cout << properties[j].Name << " : " << shortProp << " : sal_Bool" << endl;
        }
        else if(any >>= boolProp){
            cout << properties[j].Name << " : " << (boolProp ? "true" : "false") << " : sal_Bool" << endl;
        } else if(any >>= longProp){
            cout << properties[j].Name << " : " << longProp << " : sal_Bool" << endl;
        }else {
            cout << properties[j].Name << ":  " << OUStringToOString(any.getValueTypeName(),RTL_TEXTENCODING_UTF8) << endl;
        }
        
    }
    
    
}





void set_row(Sequence<Any>& row, const Transaction trans ){
    row[DATE_COLUMN] <<= static_cast<double>((sys_days(trans.date) - sys_days(year_month_day(year(1899), month(12), day(30)))).count());
    row[NAME_COLUMN] <<= OUString::createFromAscii(trans.name.c_str());
    row[MERCHANT_NAME_COLUMN] <<= OUString::createFromAscii(trans.merchant_name.c_str());
    row[AMOUNT_COLUMN] <<= std::stod(trans.amount.to_sci());
    row[PENDING_COLUMN] <<= sal_Int32(trans.pending);
    row[TRANSACTION_ID_COLUMN] <<= OUString::createFromAscii(trans.transaction_id.c_str());
    row[ORIGINAL_DESCRIPTION_COLUMN] <<= OUString::createFromAscii(trans.original_description.c_str());
}
