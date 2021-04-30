/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKCOMPILER_H
#define BLOCKCOMPILER_H
#include <string>
#include <vector>

#include "blockspec.h"

namespace rapidxml{
    template<class Ch>class xml_document;
    template<class Ch>class xml_node;
}
using xml_doc = rapidxml::xml_document<char>;
using xml_node = rapidxml::xml_node<char>;

class BlockCompiler{
public:
    static BlockCompiler& bc();
    BlockCompiler(BlockCompiler const&) = delete;
    void operator=(BlockCompiler const&) = delete;

    void setCppCompiler(const std::string& compiler);

    //Atomic blocks
    bool buildAtom(const std::string& atomPath);
    bool readAtom(const std::string& atomPath, AtomSpec& atom);
    bool writeAtom(const std::string& atomPath, const AtomSpec& atom);
    //Composite blocks
    bool buildComp(const std::string& compPath);
    bool readComp(const std::string& compPath, CompSpec& comp);
    bool writeComp(const std::string& compPath, const CompSpec& comp);

    bool openLibrary(const char* path);
protected:
    BlockCompiler();

private:
    //Atomic blocks
    bool buildAtom(const std::string& headerPath, const AtomSpec& atom);
    //Composite blocks
    bool buildComp(const std::string& headerPath, const CompSpec& comp);

    //Library initialization
    bool initLibrary();

    std::string m_libPath;
    std::string m_cppCompiler = "g++ ";
    std::string m_cppFlags = " -std=c++17 ";
};

#endif // BLOCKCOMPILER_H
