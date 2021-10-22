#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    string tempText, text;
    ifstream file("example_1.3.txt");
    if (file.is_open())
    {
        while (getline(file, tempText))
        {
            text = text + tempText + "\n";
        }
        text = text.substr(0, text.length() - 1);
        file.close();
    }
    else
        cout << "Open file error!" << endl;

    ofstream fileCopy("example_1.3_copy.txt");
    if (fileCopy.is_open())
    {
        fileCopy << text;
        fileCopy.close();
    }
    else
        cout << "Open file error!" << endl;

    return 0;
}