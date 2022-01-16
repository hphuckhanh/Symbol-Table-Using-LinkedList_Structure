#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;
not
void test(string filename)
{
    SymbolTable *st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    delete st;
}

int main(int argc, char **argv)
{
    //if (argc < 2)
      //  return 1;
    test("Text.txt");
    return 0;
}