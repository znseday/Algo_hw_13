#include <iostream>
#include <cstring>
#include <string>

#include "MyRLE.h"
#include "simpletest.h"

using namespace std;

void PrintHelp();


int main(int argc, char **argv)
{
//    IsDebugOutput = true;
//    SimpleTests();
//    return 0;

    if (argc < 2 || argc > 5)
    {
        cout << "Wrong numbers of arguments" << endl;
        PrintHelp();
        return 0;
    }

    bool isCompression = false;
    bool isDecompression = false;
    bool isImprovedRLE = false;
    string inputFileName;
    string outputFileName;

    for (int i = 1; i < argc; i++)
    {
        if ( strcmp(argv[i], "-c") == 0 && i < argc-2)
        {
            inputFileName  = argv[i+1];
            outputFileName = argv[i+2];
            isCompression = true;
        }

        if ( strcmp(argv[i], "-d") == 0 && i < argc-2)
        {
            inputFileName  = argv[i+1];
            outputFileName = argv[i+2];
            isDecompression = true;
        }

        if ( strcmp(argv[i], "-i") == 0 )
            isImprovedRLE = true;
    }

    if ( (isCompression ^ isDecompression) == 0 || inputFileName == "" || outputFileName == "")
    {
        cout << "Wrong arguments" << endl;
        PrintHelp();
        return 0;
    }

    cout << "You chose:" << endl;
    cout << boolalpha << "isCompression = " << isCompression << endl;
    cout << "isDecompression = " << isDecompression << endl;
    cout << "isImprovedRLE = " << isImprovedRLE << endl;
    cout << "Input File Name = " << inputFileName << endl;
    cout << "Output File Name = " << outputFileName << endl;

    MyRLE myRLE(inputFileName, outputFileName, isImprovedRLE);

    if (isCompression)
    {
        if ( !myRLE.Compress() )
            cout << "Compression failed!" << endl;
        else
        {
            cout << "Compression Done" << endl;
            cout << "Compression size (%): " << ((double)myRLE.GetOutpuSize())/(double)myRLE.GetInputSize()*100.0 << endl;
        }
    }

    if (isDecompression)
    {
        if ( !myRLE.Decompress() )
            cout << "Decompression failed!" << endl;
        else
           cout << "Decompression Done!" << endl;
    }


    return 0;
}

void PrintHelp()
{
    cout << "Usage:\n" <<
            "-h - show this message\n" <<
            "-c <input file> <output file> - compression <input file> into <output file>\n" <<
            "-d <input file> <output file> - decompression <input file> into <output file>\n" <<
            "-i - use improved RLE\n" <<
            "Example of usage: prog.exe -c file.txt file.rle -i" << endl;
}
