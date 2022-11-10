#include "global.h"
/**
 * @brief File contains method to process SORT commands.
 *
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * R <- SORT relation_name BY column_name IN sorting_order BUFFER buffer_size
 *
 * sorting_order = ASC | DESC
 */
bool syntacticParseSORT()
{
    logger.log("syntacticParseSORT");

    if (tokenizedQuery.size() != 8 && tokenizedQuery.size() != 10)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    if (tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    string sortingStrategy = tokenizedQuery[7];
    if (sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if (sortingStrategy == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }

    if (tokenizedQuery.size() == 10)
    {

        if (tokenizedQuery[8] != "BUFFER")
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }

        try
        {
            parsedQuery.sortBufferSize = stoi(tokenizedQuery[9]);
        }
        catch (...)
        {
            cout << "SYNTAX ERROR" << endl;
            return false;
        }
    }

    BLOCK_COUNT = parsedQuery.sortBufferSize;
    return true;
}

bool semanticParseSORT()
{
    logger.log("semanticParseSORT");

    if (tableCatalogue.isTable(parsedQuery.sortResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.sortRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName))
    {
        cout << "SEMANTIC ERROR: Column doesn't exist in relation" << endl;
        return false;
    }

    return true;
}

void executeSORT()
{
    logger.log("executeSORT");
    // cout << "You made it" << endl;

    Table table = *(tableCatalogue.getTable(parsedQuery.sortRelationName));
    int number_of_blocks = ceil(table.rowCount * 1.0 / table.maxRowsPerBlock);

    int column_index = table.getColumnIndex(parsedQuery.sortColumnName);

    int number_of_runs = ceil(number_of_blocks * 1.0 / BLOCK_COUNT);
    int blocks_read = 0;

    vector<Page> runs(number_of_runs);
    vector<int> blocksInrun(number_of_runs, 0);

    // ---------------
    //   INITIAL RUN
    // ---------------

    for (int i = 0; i < number_of_runs; i++)
    {

        vector<vector<int>> rows;
        int blocks_to_read = min((int)BLOCK_COUNT, number_of_blocks - blocks_read);

        for (int j = blocks_read; j < blocks_read + blocks_to_read; j++)
        {

            Page page_j = bufferManager.getPage(table.tableName, j);
            vector<vector<int>> rows_in_page_j = page_j.getBlock();
            rows.insert(rows.end(), rows_in_page_j.begin(), rows_in_page_j.end());
        }

        sort(rows.begin(), rows.end(), [column_index](vector<int> a, vector<int> b)
             {
            if(parsedQuery.sortingStrategy == ASC)
                return a[column_index] < b[column_index];
            else
                return a[column_index] > b[column_index]; });

        runs[i].writeBlock(rows, i, table.maxRowsPerBlock, table.tableName, 0);
        blocks_read += blocks_to_read;
        blocksInrun[i] = blocks_to_read;
    }
    int dm = min((int)BLOCK_COUNT - 1, number_of_runs);
    int point = 0;
    int pindex = 0;
    string name=parsedQuery.sortResultRelationName;
    Table *result =new Table(parsedQuery.sortResultRelationName,table.columns);
    vector<vector<int>> res;
    int phase = 0;

    while (number_of_runs != 1)
    {
        point = 0;
        vector<int> runpointers(number_of_runs, 0);
        vector<int> pointers(number_of_runs, 0);
        int total_iter = ceil(number_of_runs * 1.0 / dm);
        for (int i = 0; i < total_iter; i++)
        {
            int pind=0;
            int flag = 1;
            while (flag == 1)
            {
                flag = 0;
                vector<vector<vector<int>>> blocks(BLOCK_COUNT);
                for (int ind = point; ind < point + min(dm, number_of_runs - point); ind++)
                {

                    if (runpointers[ind] < blocksInrun[ind])
                    {
                        Page runblock = bufferManager.getPage(table.tableName, runpointers[ind], ind, phase);
                        vector<vector<int>> pageblock = runblock.getBlock();
                        blocks[ind - point] = pageblock;
                        flag = 1;
                    }
                }

                int mi_pointer = -1;
                int mi = INT_MAX;
                if(parsedQuery.sortingStrategy==DESC){
                    mi=INT_MIN;


                }

                for (int ind = point; ind < point + min(dm, number_of_runs - point); ind++)
                {
                    if (runpointers[ind] < blocksInrun[ind])
                    {
                        if(parsedQuery.sortingStrategy==ASC){
                            if (blocks[ind - point][pointers[ind]][column_index] < mi)
                            {
                                mi_pointer = ind;
                                mi = blocks[ind - point][pointers[ind]][column_index];
                            }
                        }
                        else{
                            if (blocks[ind - point][pointers[ind]][column_index] > mi)
                            {
                                mi_pointer = ind;
                                mi = blocks[ind - point][pointers[ind]][column_index];
                            }

                        }
                    }
                }
                if (mi_pointer != -1)
                {
                    res.push_back(blocks[mi_pointer - point][pointers[mi_pointer]]);
                    if (res.size() == table.maxRowsPerBlock)
                    {
                        Page p = Page();
                        if(total_iter!=1){
                            p.writeBlock(res, pind,i, table.maxRowsPerBlock, table.tableName, phase+1);
                            res.clear();
                            pind+=1;
                        }
                        else{
                            p.writeBlock(res, pind, table.maxRowsPerBlock,result->tableName);
                            res.clear();
                            pind+=1;

                        }
                    }
                    pointers[mi_pointer] += 1;
                    if (blocks[mi_pointer - point].size() == pointers[mi_pointer])
                    {
                        runpointers[mi_pointer] += 1;
                        pointers[mi_pointer] = 0;
                    }
                }
            }

            for (int ind = point; ind < point + min(dm, number_of_runs - point); ind++)
            {
                for (int pageind = 0; pageind < blocksInrun[ind]; pageind++)
                {
                    string pageName = "../data/temp/" + table.tableName + "_phase" + to_string(phase) + "_run" + to_string(ind) + "_Page" + to_string(pageind);
                    bufferManager.deleteFile(pageName);
                }
            }
            if (res.size() != 0)
            {
                Page p = Page();
                if(total_iter!=1){
                    p.writeBlock(res,pind, i, table.maxRowsPerBlock, table.tableName, phase+1);
                    pind+=1;
                    res.clear();
                }
                else{
                    p.writeBlock(res,pind, table.maxRowsPerBlock,result->tableName);
                    pind+=1;
                    res.clear();

                }
            }
            blocksInrun[i] = pind;
            pind=0;
            point += dm;
            if (point >= number_of_runs)
            {
                break;
            }
        }
        phase += 1;
        

        number_of_runs = total_iter;
    }
    result->blockCount=table.blockCount;
    result->maxRowsPerBlock=table.maxRowsPerBlock;
    result->rowsPerBlockCount=table.rowsPerBlockCount;
    result->rowCount=table.rowCount;
    result->columnCount=table.columnCount;result->columns=table.columns;
    tableCatalogue.insertTable(result);

    BLOCK_COUNT = 3;
}
