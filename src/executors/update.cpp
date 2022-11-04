#include "global.h"
/**
 * @brief 
 * SYNTAX: UPDATE relation_name COLUMN column_name operator value
 */


bool syntacticParseUPDATE()
{
    logger.log("syntacticParseSOURCE");
    if (tokenizedQuery.size() != 6)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = UPDATE;
    parsedQuery.column_name=tokenizedQuery[3];
    parsedQuery.update_value=stoi(tokenizedQuery[5]);
    parsedQuery.sourceFileName = tokenizedQuery[1];
    return true;
}

bool semanticParseUPDATE()
{
    logger.log("semanticParseSOURCE");
    cout<<parsedQuery.sourceFileName<<endl;
    if (!tableCatalogue.isTable(parsedQuery.sourceFileName))
    {
        cout << "SEMANTIC ERROR: File doesn't exist" << endl;
        return false;
    }
    if(tokenizedQuery[4]=="MULTIPLY"){
         parsedQuery.operator_=MULTIPLY;
    }
    else if(tokenizedQuery[4]=="SUBTRACT"){
        parsedQuery.operator_=SUBTRACT;
    }
    else if(tokenizedQuery[4]=="ADD"){
        parsedQuery.operator_=ADD;
    }
    else{
         cout << "SEMANTIC ERROR: Invaid opertor" << endl;
         return false;
    }
    return true;
}

void executeUPDATE(){
    return;
}