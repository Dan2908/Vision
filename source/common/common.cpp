#include "include/common.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Vision
{
namespace Util
{

//********************************
//      Class FileString
//********************************
//----------------------------------------------------------------
FileString:: FileString(const char* path) 
    : std::string("")
{
    ChangeFile(path);
}

//----------------------------------------------------------------
void FileString::ChangeFile(const char* path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream inStream;
        inStream << file.rdbuf();
        file.close();

        this->assign(inStream.str());
    }
    catch (std::ifstream::failure& e)
    {
        LOG_STDERR("Failed reading file \'" << path << "\': " << e.what());
    }
}

} // namespace System
} // namespace Vision
