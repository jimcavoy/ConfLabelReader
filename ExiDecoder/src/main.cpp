#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <exi2xml/XmlWriter.h>

int main(int argc, char* argv[])
{
    using namespace std;
    
    ifstream exiFile;

    if (argc != 2)
    {
        cerr << "Usage: ExiDecoder <filepath>" << endl;
        return -1;
    }

    exiFile.open(argv[1], ios::in|ios::binary|ios::ate);
    if (!exiFile)
    {
        cerr << "Fail to open file " << argv[1];
        return -1;
    }

    streampos size = exiFile.tellg();
    char* buff = new char[size];
    memset(buff, 0, size);
    exiFile.seekg(0, ios::beg);
    exiFile.read(buff, size);
    exiFile.close();

    stringstream out;
    ThetaStream::XmlWriter decoder(out);
    decoder.decode(buff, size);

    cout << out.str();
    return 0;
}