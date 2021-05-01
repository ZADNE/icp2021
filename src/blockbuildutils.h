/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKBUILDUTILS_H
#define BLOCKBUILDUTILS_H
#include <fstream>

#include "blockspec.h"

namespace BlockBuildUtils{

    void writeTemplates(std::ofstream& o, const SlotList& inputs, const SlotList& outputs);
    void writeGuardStartAndLib(std::ofstream& o, std::string blockName);
    void writeInstanceIncludes(std::ofstream& o, const InstanceList& il);

    void writePortNameSetters(std::ofstream& o, const std::string& instance, const SlotList& outputs);
    void writePortNameSettersForAtomBlocks(std::ofstream& o, const InstanceList& il);
    void writeConnections(std::ofstream& o, const ConnectionList& cl);
    void writeConstValueSetters(std::ofstream& o, const ConstantList& cl);

    std::string qualifiedPortname(const std::string& instance, const std::string& port);
}

#endif // BLOCKBUILDUTILS_H
