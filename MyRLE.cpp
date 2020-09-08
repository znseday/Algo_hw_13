#include "MyRLE.h"

#include <iostream>
#include <fstream>


using namespace std;

bool IsDebugOutput = false;

MyRLE::MyRLE(string _inputFileName, string _outputFileName, bool _isImprovedRLE)
    : InputFileName(_inputFileName), OutputFileName(_outputFileName), IsImprovedRLE(_isImprovedRLE)
{

}
//-------------------------------------------------------

bool MyRLE::Compress()
{
    ifstream in_f(InputFileName, ios::binary);
    if (!in_f)
    {
        std::perror(InputFileName.c_str());
        return false;
    }

    in_f.seekg(0, ios::end);
    InputSize = in_f.tellg();
    in_f.seekg(0, ios::beg);

    vector<char> buf(InputSize);
    in_f.read(buf.data(), InputSize);
    in_f.close();

    auto res = CompressBuffer(buf, IsImprovedRLE);

    ofstream out_f(OutputFileName, ios::binary);
    if (!out_f)
    {
        std::perror(OutputFileName.c_str());
        return false;
    }

    out_f.write(res.data(), res.size());
    OutpuSize = res.size();

    out_f.close();
    return true;
}
//-------------------------------------------------------

bool MyRLE::Decompress()
{
    ifstream in_f(InputFileName, ios::binary);
    if (!in_f)
    {
        std::perror(InputFileName.c_str());
        return false;
    }
    in_f.seekg(0, ios::end);
    InputSize = in_f.tellg();
    in_f.seekg(0, ios::beg);

    vector<char> buf(InputSize);
    in_f.read(buf.data(), InputSize);
    in_f.close();

    auto res = DecompressBuffer(buf, IsImprovedRLE);

    ofstream out_f(OutputFileName, ios::binary);
    if (!out_f)
    {
        std::perror(OutputFileName.c_str());
        return false;
    }

    out_f.write(res.data(), res.size());
    OutpuSize = res.size();

    out_f.close();
    return true;
}
//-------------------------------------------------------

void FlushNegative(const vector<char> &buf, vector<char> &newBuf, size_t countN, size_t i)
{
    newBuf.emplace_back(-countN);
    MY_DEBUG_ONLY( cout << (int)newBuf.back() << " - "; )
    for (size_t k = 0; k < countN; ++k)
    {
        newBuf.emplace_back(buf.at(i-countN+k));
        MY_DEBUG_ONLY( cout << newBuf.back(); )
    }
    MY_DEBUG_ONLY( cout << endl; )
    countN = 0;
}

vector<char> MyRLE::CompressBuffer(const vector<char> &buf, bool isImproved)
{
    vector<char> newBuf;
    newBuf.reserve(buf.size()*2);
    size_t limitP = 127;
    constexpr size_t limitN = 128;

    if (!isImproved)
    {
        limitP = 255;
        for (size_t i = 0, count; i < buf.size(); i += count)
        {
            count = 0;
            while (1)
            {
                count++;
                if ( (i+count >= buf.size()) || (buf.at(i+count) != buf[i]) || count >= limitP)
                    break;
            }
            if (count == 0)
                throw runtime_error("count == 0");

            if (count >= 256)
                throw runtime_error("count >= 256 : " + to_string(count));

            newBuf.emplace_back(count);
            if (newBuf.back() == 0)
                throw runtime_error("count == 0");

            newBuf.emplace_back(buf[i]);

            MY_DEBUG_ONLY( cout << count << " - " << buf[i] << endl; )
        }
    }
    else // Improved RLE
    {
        bool same = false;
        if (buf.size() > 2)
            if (buf[0] == buf[1] && buf[0] == buf[2])
                same = true;

        size_t countN = 0;
        for (size_t i = 0, countP; i < buf.size(); /*i += (!same?countP:1)*/)
        {
            if (countN >= limitN)
            {
                FlushNegative(buf, newBuf, countN, i);
                countN = 0;
            }

            if (i < buf.size()-2)
                if (buf[i] == buf[i+1] && buf[i] == buf[i+2])
                {
                    same = true;
                    if (countN > 0)
                    {
                        FlushNegative(buf, newBuf, countN, i);
                    }
                }

            if (same)
            {
                countP = countN = 0;
                while (1)
                {
                    countP++;
                    if ( (i+countP >= buf.size()) || (buf.at(i+countP) != buf[i]) || countP >= limitP)
                        break;
                }

                newBuf.emplace_back(countP);
                newBuf.emplace_back(buf[i]);
                MY_DEBUG_ONLY( cout << countP << " - " << buf[i] << endl; )
                same = false;
                i += countP;
            }
            else
            {
                countP = 0;
                countN++;
                i++;
            }
        }
        if (!same && countN > 0)
        {
            FlushNegative(buf, newBuf, countN, buf.size());
        }
    }

    newBuf.shrink_to_fit();
    return newBuf;
}
//-------------------------------------------------------

vector<char> MyRLE::DecompressBuffer(const vector<char> &buf, bool isImproved)
{
    size_t bufSize = buf.size();

    vector<char> newBuf;
    newBuf.reserve(bufSize*10);

    if (!isImproved)
    {
        for (size_t i = 0; i < bufSize; i+=2)
        {
            size_t c = (unsigned char)buf[i];
            if (c > 255)
                throw runtime_error("c > 255");
            if (c <= 0)
                throw runtime_error("c <= 0: " + to_string(c));
            for (size_t k = 0; k < c; ++k)
            {
                newBuf.push_back(buf[i+1]);

//                if (newBuf.size() > bufSize)
//                    throw runtime_error("newBuf.size() > bufSize");
            }
        }
    }
    else // Improved RLE
    {
        for (size_t i = 0; i < bufSize; /*i+=2*/)
        {
            if (buf[i] < 0)
            {
                int c = abs(buf[i]);
                if (c > 129)
                    throw runtime_error("c > 129");
                for (size_t k = 0; (int)k < c; ++k)
                {
                    newBuf.emplace_back(buf[i+1+k]);
                }
                i += c+1;
            }
            else
            {
                for (size_t k = 0; k < (unsigned char)buf[i]; ++k)
                {
                    newBuf.emplace_back(buf[i+1]);
                }
                i += 2;
            }

        }
    }

    return newBuf;
}
//-------------------------------------------------------
