#include"logger.h"
/**
 * @brief The Page object is the main memory representation of a physical page
 * (equivalent to a block). The page class and the page.h header file are at the
 * bottom of the dependency tree when compiling files. 
 *<p>
 * Do NOT modify the Page class. If you find that modifications
 * are necessary, you may do so by posting the change you want to make on Moodle
 * or Teams with justification and gaining approval from the TAs. 
 *</p>
 */

class Page{

    string tableName;
    string pageIndex;
    int columnCount;
    int rowCount;
    vector<vector<int>> rows;

    public:
    void update_columns(string tableName, int pageIndex, string column);

    string pageName = "";
    Page();
    Page(string tableName, int pageIndex);
    Page(string tableName, int pageIndex,int runIndex,int phaseIndex);
    Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
    vector<int> getRow(int rowIndex);
    vector<vector<int>> getBlock();
    void writeBlock(vector<vector<int>> rows, int run_no, int bfr, string tableName,int phaseIndex);
    void writeBlock(vector<vector<int>> rows,int pageindex, int run_no, int bfr, string tableName,int phaseIndex);
    void writeBlock(vector<vector<int>> rows,int pageindex, int bfr, string tableName);
    void writePage();
};