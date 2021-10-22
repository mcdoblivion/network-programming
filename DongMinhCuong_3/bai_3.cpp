// Dong Minh Cuong - 18020255

#include <iostream>
#include <fstream>
#include <cstring>
#define MAX_NAME_LENGTH 100

using namespace std;

struct udphdr
{
    unsigned short fileSize;
    unsigned char fileType;
    unsigned char fileNameLength;
    char fileName[MAX_NAME_LENGTH];
};

int getFileSize(string fileName)
{
    ifstream in_file(fileName, ios::binary);
    in_file.seekg(0, ios::end);
    return in_file.tellg();
}

int main()
{
    struct udphdr udphdrInstance;
    int temp;

    cout << "Enter file size: ";
    cin >> temp;
    udphdrInstance.fileSize = (unsigned short)temp;

    cout << "Enter file type: ";
    cin >> temp;
    udphdrInstance.fileType = (unsigned char)temp;

    string data;
    cout << "Enter file name: ";
    cin.ignore();
    getline(cin, data);
    strcpy(udphdrInstance.fileName, data.c_str());

    fstream outputFile;
    outputFile.open("pkt_hdr_json.txt", ios::out | ios::binary);

    outputFile << "{" << endl;
    outputFile << "\"fileSize\": " << udphdrInstance.fileSize << "," << endl;
    outputFile << "\"fileType\": " << udphdrInstance.fileType << "," << endl;
    outputFile << "\"fileName\": " << udphdrInstance.fileName << "," << endl;
    outputFile << "}" << endl;

    outputFile.close();

    cout << "Size of pkt_hdr_json.txt: " << getFileSize("pkt_hdr_json.txt") << endl;
    cout << "Size of pkt_hdr.bin: " << getFileSize("pkt_hdr.bin") << endl;

    return 0;
}