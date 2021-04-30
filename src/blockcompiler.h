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

    bool openLibrary(const char* path);

    void setCppCompiler(const std::string& compiler);

    //Atomic blocks
    bool buildAtom(const std::string& atomPath);
    bool readAtom(const std::string& atomPath, AtomSpec& atom);
    bool writeAtom(const std::string& atomPath, const AtomSpec& atom);
    //Composite blocks
    bool buildComp(const std::string& compPath);
    bool readComp(const std::string& compPath, CompSpec& comp);
    bool writeComp(const std::string& compPath, const CompSpec& comp);
    //Applications
    bool buildAppl(const std::string& applPath);
    bool readAppl(const std::string& applPath, ApplSpec& appl);
    bool writeAppl(const std::string& applPath, const ApplSpec& appl);

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
