/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#ifndef BLOCKCOMPILER_H
#define BLOCKCOMPILER_H
#include <string>
#include <vector>

struct SlotSpec{
    std::string type;
    std::string name;
};

using SlotList = std::vector<SlotSpec>;

struct AtomSpec{
    std::string name;
    SlotList inputs;
    SlotList outputs;
    std::string body;
};

class BlockCompiler{
public:
    BlockCompiler();

    static BlockCompiler& bc();

    bool buildAtom(const std::string& filePath, const AtomSpec& atom);

    bool openLibrary(const char* path);
private:
    std::string m_libPath;

    bool checkHeader();
};

#endif // BLOCKCOMPILER_H
