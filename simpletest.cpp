#include "simpletest.h"

#include <iostream>
#include <vector>
#include <cstring>

#include "MyRLE.h"

using namespace std;

void SimpleTest(const char *str)
{
    cout << "Simple RLE:" << endl;
    size_t bufSize = strlen(str);
    vector<char> buf(bufSize, '\0');
    memcpy(buf.data(), str, bufSize);
    cout << "Src data str: ";
    for (auto c : buf) cout << c;
    cout << endl;

    auto comressedBuf = MyRLE::CompressBuffer(buf, false); // simple RLE
    auto decompressedBuf = MyRLE::DecompressBuffer(comressedBuf, false);// simple RLE
    cout << "Decompressed: ";
    for (auto c : decompressedBuf) cout << c;
    cout << endl;

    if (buf != decompressedBuf)
        throw runtime_error("buf != decompressedBuf (simple RLE)");

    cout << endl << "Improved RLE:" << endl;
    cout << "Src data str: ";
    for (auto c : buf) cout << c;
    cout << endl;

    comressedBuf = MyRLE::CompressBuffer(buf, true); // improved RLE
    decompressedBuf = MyRLE::DecompressBuffer(comressedBuf, true); // improved RLE
    cout << "Decompressed: ";
    for (auto c : decompressedBuf) cout << c;
    cout << endl;
    if (buf != decompressedBuf)
        throw runtime_error("buf != decompressedBuf (improved RLE)");
    cout << endl;
}

void SimpleTests()
{
    cout << "SimpleTest started";

    cout << endl << "Test1:" << endl;
    SimpleTest("AAABCCDEEEEEEEEEEEEEEEEEFGHXYZ");

    cout << endl << "Test2:" << endl;
    SimpleTest("AAABCCDEEEEEEEEEEEEEEEEEEEEEEE");

    cout << endl << "Test3:" << endl;
    SimpleTest("ABCCDDDDEEEEEFFFFFFFGGGEEEXYZZ");

    cout << endl << "Test4:" << endl;
    SimpleTest("AAAAAAABCDEFGHEEFGGEXXYYYYYYYZ");

    cout << endl << "Test5:" << endl;
    constexpr auto N = 201;
    char str[N];
    for (auto &c : str)
        c = 1 + rand()%255;
    str[N-1] = '\0';
    SimpleTest(str);

    cout << "SimpleTest finished" << endl;
}
