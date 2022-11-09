#include"global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * R <- SORT relation_name BY column_name IN sorting_order BUFFER buffer_size
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
  
    if (tokenizedQuery.size() != 8 && tokenizedQuery.size() != 10) {
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }

    if(tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    string sortingStrategy = tokenizedQuery[7];
    if(sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrategy == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
  
    if(tokenizedQuery.size() == 10) {
        
        if(tokenizedQuery[8] != "BUFFER") {
            cout<<"SYNTAX ERROR"<<endl;
            return false;
        }

        try
        {
            parsedQuery.sortBufferSize = stoi(tokenizedQuery[9]);
        }
        catch (...)
        {
            cout<< "SYNTAX ERROR" << endl;
            return false;
        }
    }

    BLOCK_COUNT = parsedQuery.sortBufferSize;
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}

void mergeSort()
{}


void executeSORT(){
    logger.log("executeSORT");
    // cout << "You made it" << endl;
    
    Table table = *(tableCatalogue.getTable(parsedQuery.sortRelationName));
    int number_of_blocks = ceil(table.rowCount * 1.0 / table.maxRowsPerBlock);

    int column_index = table.getColumnIndex(parsedQuery.sortColumnName);

    int number_of_runs = ceil(number_of_blocks * 1.0 / BLOCK_COUNT);
    int blocks_read = 0;

    vector<Page> runs(number_of_runs);

    // ---------------
    //   INITIAL RUN
    // ---------------


    for(int i = 0; i < number_of_runs; i++) {

        vector<vector<int>> rows;
        int blocks_to_read = min((int)BLOCK_COUNT, number_of_blocks - blocks_read);  

        for(int j = blocks_read; j < blocks_read + blocks_to_read; j++) {
            
            Page page_j = bufferManager.getPage(table.tableName, j);
            vector<vector<int>> rows_in_page_j = page_j.getBlock();
            cout << page_j.pageName << endl;
            rows.insert(rows.end(), rows_in_page_j.begin(), rows_in_page_j.end());
        }

        sort(rows.begin(), rows.end(), [column_index](vector<int> a, vector<int> b) {
            if(parsedQuery.sortingStrategy == ASC)
                return a[column_index] < b[column_index];
            else
                return a[column_index] > b[column_index];
        });


        // Write the run file to disk
        runs[i].writeBlock(rows, i, table.maxRowsPerBlock, table.tableName);
        blocks_read += blocks_to_read;
    }

    


    // Revert BLOCK_COUNT to original value
    BLOCK_COUNT = 3;
}
