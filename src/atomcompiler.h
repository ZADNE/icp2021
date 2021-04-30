/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef ATOMCOMPILER_H
#define ATOMCOMPILER_H
#include <string>

#include "blockspec.h"

class AtomCompiler{
friend class BlockCompiler;
public:
    AtomCompiler(AtomCompiler const&) = delete;
    void operator=(AtomCompiler const&) = delete;

    bool buildAtom(const std::string& headerPath, const AtomSpec& atom);

protected:
    AtomCompiler();
};

#endif // ATOMCOMPILER_H
