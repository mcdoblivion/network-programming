// Dong Minh Cuong - 18020255

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#define MAX_NAME_LENGTH 100

struct udphdr
{
    unsigned short fileSize;
    unsigned char fileType;
    unsigned char fileNameLength;
    char fileName[MAX_NAME_LENGTH];
};

int main()
{
    udphdr udphdrInstance;
    ifstream file("pkt_hdr.bin", ios::out | ios::binary);
    file.read((char *)&udphdrInstance, sizeof(udphdrInstance));
    cout << "File size: " << udphdrInstance.fileSize << endl;
    cout << "File type: " << udphdrInstance.fileType << endl;
    cout << "File name length: " << udphdrInstance.fileNameLength << endl;
    cout << "File name: " << udphdrInstance.fileName << endl;

    file.close();

    return 0;
}