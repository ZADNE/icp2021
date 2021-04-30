/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef COMPCOMPILER_H
#define COMPCOMPILER_H
#include <string>

#include "blockspec.h"

class CompCompiler{
friend class BlockCompiler;
public:
    CompCompiler(CompCompiler const&) = delete;
    void operator=(CompCompiler const&) = delete;

    //May throw
    bool buildComp(const std::string& headerPath, const CompSpec& comp);

protected:
    CompCompiler(const std::string& libPath);

private:
    const std::string& m_libPath;
};

#endif // COMPCOMPILER_H
