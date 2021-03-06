/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKBUILDER_H
#define BLOCKBUILDER_H
#include <string>
#include <set>

#include "rapidxml/rapidxml.hpp"

#include "blockspec.h"
#include "atombuilder.h"
#include "compbuilder.h"
#include "applbuilder.h"

///
/// \brief Builder for all block files (*.atom, *.comp, *.appl)
///
class BlockBuilder{
public:
    static BlockBuilder& get();
    BlockBuilder(BlockBuilder const&) = delete;
    void operator=(BlockBuilder const&) = delete;

    bool openLibrary(const std::string& path);

    void setCppCompiler(const std::string& compiler);

    //Atomic blocks
    bool buildAtom(const std::string& atomPath);
    //Composite blocks
    bool buildComp(const std::string& compPath, std::set<std::string>* toBeBuilt = nullptr);
    //Applications
    bool buildAppl(const std::string& applPath);

protected:
    BlockBuilder();

private:
    //Library initialization
    bool initLibrary();

    std::string m_libPath;
    std::string m_cppCompiler = "g++ ";
    std::string m_cppFlags = " -std=c++17 ";

    AtomBuilder atomB{m_libPath};
    CompBuilder compB{m_libPath};
    ApplBuilder applB{m_libPath};
};

#endif // BLOCKBUILDER_H
