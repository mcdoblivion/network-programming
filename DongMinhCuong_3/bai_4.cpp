// Dong Minh Cuong - 18020255

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main()
{
    string text, line;

    ifstream file("pkt_hdr_json.txt");

    while (getline(file, line))
    {
        text = text + line + "\n";
    }

    file.close();

    cout << text << endl;

    for (int i = 0; i < text.length(); i++){
        if (text[i] == ':') {
            string temp;
            while (text[++i] != ',') {
                temp += text[i];
            }
            cout << temp << endl;
        }
    }
    
    return 0;
}
