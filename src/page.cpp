#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName + "_Page" + to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    uint maxRowCount = table.maxRowsPerBlock;
    vector<int> row(columnCount, 0);

    ifstream fin(pageName, ios::in);
    this->rowCount = table.rowsPerBlockCount[pageIndex];
    this->rows.assign(this->rowCount, row);
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}


Page::Page(string tableName, int pageIndex,int runIndex,int phaseIndex)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->tableName +"_phase"+to_string(phaseIndex)+ "_run" + to_string(runIndex) +"_Page"+ to_string(pageIndex);
    Table table = *tableCatalogue.getTable(tableName);
    this->columnCount = table.columnCount;
    vector<int> row(columnCount, 0);
    ifstream fin(pageName, ios::in);
    fin>>this->rowCount;
    // cout<<this->rowCount<<"_____________________"<<pageName<<endl;
    this->rows.assign(this->rowCount, row);
    int number;
    for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}
/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}


vector<vector<int>> Page::getBlock() {
    logger.log("Page::getBlock");
    return this->rows;
}

void Page::writeBlock(vector<vector<int>> rows, int run_no, int bfr, string tableName ,int phaseIndex) {
    
    // Number of blocks for the run file
    int no_of_blocks = ceil(rows.size() * 1.0 / bfr);

    // cout << bfr;
    // cout << rows.size()<<endl;
    auto begin = rows.begin();

    for(int i = 0; i < no_of_blocks; i++) {
        
        string run_name = "../data/temp/" + tableName +"_phase"+to_string(phaseIndex)+ "_run" + to_string(run_no) + "_Page" + to_string(i);
        ofstream fout(run_name , ios::trunc);
        
        auto row_end = begin + min(bfr, (int)rows.size() - i * bfr);
        fout<<min(bfr, (int)rows.size() - i * bfr)<<endl;

        for(auto row = begin; row != row_end; row++) {
            for(auto col = row->begin(); col != row->end(); col++) {
                fout << *col << " ";
            }
            fout << endl;
        }

        begin = row_end;
        
        fout.close();
    }

}

void Page::writeBlock(vector<vector<int>> rows,int pageindex, int run_no, int bfr, string tableName ,int phaseIndex) {
    
    // Number of blocks for the run file
    int no_of_blocks = ceil(rows.size() * 1.0 / bfr);

    // cout << bfr;
    // cout << rows.size()<<endl;
    auto begin = rows.begin();

    for(int i = 0; i < no_of_blocks; i++) {
        
        string run_name = "../data/temp/" + tableName +"_phase"+to_string(phaseIndex)+ "_run" + to_string(run_no) + "_Page" + to_string(pageindex);
        // this->pageName=run_name;
        ofstream fout(run_name , ios::trunc);
        
        auto row_end = begin + min(bfr, (int)rows.size() - i * bfr);
        fout<<min(bfr, (int)rows.size() - i * bfr)<<endl;

        for(auto row = begin; row != row_end; row++) {
            for(auto col = row->begin(); col != row->end(); col++) {
                fout << *col << " ";
            }
            fout << endl;
        }

        begin = row_end;
        
        fout.close();
    }

}



void Page::writeBlock(vector<vector<int>> rows,int pageindex, int bfr, string tableName) {
    
    // Number of blocks for the run file
    int no_of_blocks = ceil(rows.size() * 1.0 / bfr);

    // cout << bfr;
    // cout << rows.size()<<endl;
    auto begin = rows.begin();

    for(int i = 0; i < no_of_blocks; i++) {
        
        string run_name = "../data/temp/" + tableName +"_Page" + to_string(pageindex);
        // this->pageName=run_name;
        ofstream fout(run_name , ios::trunc);
        
        auto row_end = begin + min(bfr, (int)rows.size() - i * bfr);
        // fout<<min(bfr, (int)rows.size() - i * bfr)<<endl;

        for(auto row = begin; row != row_end; row++) {
            for(auto col = row->begin(); col != row->end(); col++) {
                fout << *col << " ";
            }
            fout << endl;
        }

        begin = row_end;
        
        fout.close();
    }

}
Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page");
    this->tableName = tableName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}
