/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKCOMPILER_H
#define BLOCKCOMPILER_H
#include <string>
#include <vector>

namespace rapidxml {
    template<class Ch>class xml_node;
}

struct SlotSpec{
    SlotSpec(){}
    SlotSpec(bool templ, std::string type, std::string name):
        templ(templ), type(type), name(name) {};

    bool templ;
    std::string type;
    std::string name;
};

using SlotList = std::vector<SlotSpec>;

struct AtomSpec{
    std::string name;
    SlotList inputs;
    SlotList outputs;
    std::string body;
    std::string stateVars;
};

class BlockCompiler{
public:
    static BlockCompiler& bc();
    BlockCompiler(BlockCompiler const&) = delete;
    void operator=(BlockCompiler const&) = delete;

    void setCppCompiler(std::string compiler);

    bool buildAtom(const std::string& atomPath);

    bool openLibrary(const char* path);
protected:
    BlockCompiler();

private:
    bool readAtom(const std::string& atomPath, AtomSpec& atom); //Throws rapidxml::parse_error and std::runtime_error
    bool extractPorts(rapidxml::xml_node<char>* node, SlotList& sl);

    bool buildAtom(const std::string& headerPath, const AtomSpec& atom);
    bool checkLibraryHeaderHeader();

    std::string m_libPath;
    std::string m_cppCompiler = "g++ ";
    std::string m_cppFlags = " -std=c++17 ";
};

#endif // BLOCKCOMPILER_H
