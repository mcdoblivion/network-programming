#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream file("1.4.png", ios::binary);
    ofstream fileCopy("1.4_copy.png", ios::binary);
    if (file.is_open() && fileCopy.is_open())
    {
        fileCopy << file.rdbuf();

        fileCopy.close();
        file.close();
    }
    else
        cout << "Open file error!" << endl;

    return 0;
}