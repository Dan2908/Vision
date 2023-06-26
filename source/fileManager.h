#pragma once
#include <common/include/common.h>

namespace Vision
{
namespace Util
{

template<class T>
class File
{
protected:
    std::ifstream mFile;
public:
    File()
    : mFile()
{}
    void Load(const char* path)
    {
        mFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            mFile.open(path);
        }
        catch (std::ifstream::failure& e)
        {
            LOG_STDERR("Failed reading file \'" << path << "\': " << e.what());
        }
    }

    void Close()
    {
        mFile.close();
    }

    void Change(const char* path)
    {
        Close();
        Load(path);
    }

    virtual T* Read() = 0;
};


}//namespace Util
}//namespace Vision



	