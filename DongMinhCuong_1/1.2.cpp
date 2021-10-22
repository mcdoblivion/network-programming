#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    string fileName;
    cout << "Please enter a file name: " << endl;
    getline(cin, fileName);

    string input = "";
    char c;
    cout << "Please enter something, type # then type enter to exit." << endl;

    while (cin.get(c) && c != '#')
    {
        input += c;
    }

    ofstream myFile(fileName);
    if (myFile.is_open())
    {
        cout << "You've entered:\n"
             << input << "\nYour input has been saved as " << fileName << endl;
        myFile << input;
        myFile.close();

        ifstream file(fileName, ios::binary);
        streampos begin = file.tellg(), end;
        file.seekg(0, ios::end);
        end = file.tellg();
        cout << fileName << " size: " << end - begin << " bytes" << endl;
        file.close();
    }
    else
        cout << "Couldn't open " << fileName << endl;

    return 0;
}