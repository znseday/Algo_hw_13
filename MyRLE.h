#ifndef MYRLE_H
#define MYRLE_H

#include <string>
#include <vector>

extern bool IsDebugOutput;

#define MY_DEBUG_ONLY(x) { if(IsDebugOutput) {x}  }

class MyRLE
{
private:

    std::string InputFileName;
    std::string OutputFileName;
    bool IsImprovedRLE = false;

    size_t InputSize = 0;
    size_t OutpuSize = 0;

public:
    MyRLE() = delete;
    MyRLE(std::string _inputFileName, std::string _outputFileName, bool _isImprovedRLE);

    bool Compress();
    bool Decompress();

    static std::vector<char> CompressBuffer(const std::vector<char> &buf, bool isImproved);
    static std::vector<char> DecompressBuffer(const std::vector<char> &buf, bool isImproved);

    size_t GetInputSize() {return InputSize;}
    size_t GetOutpuSize() {return OutpuSize;}

};

#endif // MYRLE_H
