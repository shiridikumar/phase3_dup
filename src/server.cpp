//Server Code
#include "global.h"
#include <sys/stat.h>

using namespace std;

float BLOCK_SIZE = 1;
uint BLOCK_COUNT = 3;
uint PRINT_COUNT = 20;
struct stat buffer;
Logger logger;
vector<string> tokenizedQuery;
ParsedQuery parsedQuery;
TableCatalogue tableCatalogue;
BufferManager bufferManager;
FILE *datafile;
int fd_dfile;
int locktype;
fstream fin;
void doCommand()
{
    logger.log("doCommand");
    if (syntacticParse() && semanticParse())
        executeCommand();
    return;
}

int main(int argc, char* argv[])
{

    regex delim("[^\\s,]+");
    string command;
    if(argc==1){
        system("rm -rf ../data/temp");
    }
    string st ="../data/temp";
    if(stat(st.c_str(), &buffer) !=0){
        system("mkdir ../data/temp");
    }
    // char *st="WRITE";
    string a("WRITE");
    string b;
    if(argc > 1)
        b = argv[1];

    // cout<<1<<endl;
   if(argc>1){
       if(!(a.compare(b))){
           locktype=1;
       }
       else{
           locktype=0;
       }
   }
   else{
       locktype=-1;
   }


    while(!cin.eof())
    {
        cout << "\n> ";
        tokenizedQuery.clear();
        parsedQuery.clear();
        logger.log("\nReading New Command: ");
        getline(cin, command);
        logger.log(command);


        auto words_begin = std::sregex_iterator(command.begin(), command.end(), delim);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            tokenizedQuery.emplace_back((*i).str());

        if (tokenizedQuery.size() == 1 && tokenizedQuery.front() == "QUIT")
        {
            break;
        }

        if (tokenizedQuery.empty())
        {
            continue;
        }

        if (tokenizedQuery.size() == 1)
        {
            cout << "SYNTAX ERROR" << endl;
            continue;
        }

        doCommand();
    }
    //  flock(LOCK_UN,fd_dfile);
    if(locktype!=-1){
        if(locktype==0){
            cout<<"released shared lock_1"<<endl;
        }
        else{
            cout<<"released exclusive lock_1"<<endl;

        }
    }
}