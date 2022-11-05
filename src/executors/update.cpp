#include "global.h"
#include <sys/file.h>

/**
 * @brief 
 * SYNTAX: UPDATE relation_name COLUMN column_name operator value
 */


bool syntacticParseUPDATE()
{
    logger.log("syntacticParseSOURCE");
    if (tokenizedQuery.size() != 6)
    {
        cout<<tokenizedQuery.size()<<endl;
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
    Table table = * tableCatalogue.getTable(parsedQuery.sourceFileName);
    Cursor cursor(table.tableName, 0);
    int rowcount=table.rowCount;
    // cursor.page.update_columns(table.tableName,0,parsedQuery.column_name);
    // int ind=bufferManager.BufferIndex(table.tableName,0);
    string newSourceFile = "../data/" + table.tableName + ".csv";
    ofstream fout(newSourceFile, ios::out);
    vector<int> row;
    FILE *pFile;
    pFile = fopen(newSourceFile.c_str(), "w+");
     table.writeRow(table.columns, pFile,1);
    for(int i=0;i<table.rowCount;i++){
        // int flag=(i!=4)?1:0;
        row=cursor.getNext();
        if(parsedQuery.operator_==MULTIPLY){
            row[table.getColumnIndex(parsedQuery.column_name)]*=parsedQuery.update_value;
        }
        else if(parsedQuery.operator_==ADD){
            row[table.getColumnIndex(parsedQuery.column_name)]+=parsedQuery.update_value;
        }
        else{
            row[table.getColumnIndex(parsedQuery.column_name)]-=parsedQuery.update_value;
        }

        table.writeRow(row,pFile,0);

    }
    fclose(pFile);
    cursor.page.update_columns(table.tableName,0,parsedQuery.column_name);
    int ind=bufferManager.BufferIndex(table.tableName,0);


    return;
}