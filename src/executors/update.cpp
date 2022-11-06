#include "global.h"
#include <sys/file.h>
#include <unistd.h>
extern FILE* datafile;
extern int fd_dfile;
extern fstream fin;
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
    string newSourceFile = "../data/" + table.tableName + ".csv";
    //  sleep(5);
    vector<int> row;
    FILE *pFile;
    pFile = fopen(newSourceFile.c_str(), "w+");
    int fd=fileno(pFile);
    FILE *pFile1;
    string pagename="../data/temp/"+table.tableName + "_Page" + to_string(0);
    pFile1 = fopen(pagename.c_str(), "r+");
    int fd1=fileno(pFile1);
    // cout<<fd<<"---"<<fd1<<endl;
    int lc1=flock(fd1,LOCK_EX);
    cout<<"lock obtained"<<endl;

    Cursor cursor(table.tableName, 0);
    int rowcount=table.rowCount;
    table.writeRow(table.columns, pFile,1);
    for(int i=0;i<table.rowCount;i++){
      
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
    cout<<"unlocked"<<endl;
    fclose(pFile1);
    flock(LOCK_UN,fd_dfile);
    fclose(pFile);
    cout<<"completed update"<<endl;
    cursor.page.update_columns(table.tableName,0,parsedQuery.column_name);
    int ind=bufferManager.BufferIndex(table.tableName,0);
    return;
}