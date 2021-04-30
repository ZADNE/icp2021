/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef APPLCOMPILER_H
#define APPLCOMPILER_H
#include <string>

#include "blockspec.h"

class ApplCompiler{
friend class BlockCompiler;
public:
    ApplCompiler(ApplCompiler const&) = delete;
    void operator=(ApplCompiler const&) = delete;

    void setLibraryPath(std::string libPath);

    //May throw
    bool buildAppl(const std::string& filePath, const ApplSpec& appl);

protected:
    ApplCompiler(const std::string& libPath);

private:
    const std::string& m_libPath;
};

#endif // APPLCOMPILER_H
