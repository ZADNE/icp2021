/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKCOMPILER_H
#define BLOCKCOMPILER_H
#include <string>

#include "blockspec.h"
#include "atomcompiler.h"
#include "compcompiler.h"
#include "applcompiler.h"

namespace rapidxml{
    template<class Ch>class xml_document;
    template<class Ch>class xml_node;
}
using xml_doc = rapidxml::xml_document<char>;
using xml_node = rapidxml::xml_node<char>;

class BlockCompiler{
public:
    static BlockCompiler& get();
    BlockCompiler(BlockCompiler const&) = delete;
    void operator=(BlockCompiler const&) = delete;

    bool openLibrary(const std::string& path);

    void setCppCompiler(const std::string& compiler);

    //Atomic blocks
    bool buildAtom(const std::string& atomPath);
    //Composite blocks
    bool buildComp(const std::string& compPath);
    //Applications
    bool buildAppl(const std::string& applPath);

protected:
    BlockCompiler();

private:
    //Library initialization
    bool initLibrary();

    std::string m_libPath;
    std::string m_cppCompiler = "g++ ";
    std::string m_cppFlags = " -std=c++17 ";

    AtomCompiler atomC;
    CompCompiler compC{m_libPath};
    ApplCompiler applC{m_libPath};
};

#endif // BLOCKCOMPILER_H
