/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "blockbuildutils.h"

#include <set>
#include <algorithm>

void BlockBuildUtils::writeTemplates(std::ofstream& o, const SlotList& inputs, const SlotList& outputs){
    std::set<std::string> templates;
    for (auto& slot: inputs){
        if (slot.templ) templates.insert(slot.type);
    }
    for (auto& slot: outputs){
        if (slot.templ) templates.insert(slot.type);
    }
    if (!templates.empty()){
        o << "template<";
        auto it = templates.begin();
        o << "typename " << *it;
        ++it;
        while (it != templates.end()){
            o << ", typename " << *it;
            ++it;
        }
        o << "> ";
    }
}


void BlockBuildUtils::writeGuardStartAndLib(std::ofstream& o, std::string blockName){
    //Guardify name
    std::transform(blockName.begin(), blockName.end(), blockName.begin(), std::toupper);
    blockName.append("_H");
    //Write
    o << "#ifndef " << blockName << '\n';
    o << "#define " << blockName << '\n';
    o << "#include \"library.hpp\"\n";
}

void BlockBuildUtils::writeInstanceIncludes(std::ofstream& o, const InstanceList& il){
    for (auto& inst: il){
        o << "#include \"" << inst.path << ".hpp\"\n";
    }
    o << '\n';
}
void BlockBuildUtils::writePortNameSetters(std::ofstream& o, const std::string& instance, const SlotList& outputs){
    for (auto& out: outputs){
        std::string name = BlockBuildUtils::qualifiedPortname(instance, out.name);
        o << "\t\tsetName(&" << name << ", ____name + \"" << name << "\");\n";
    }
}

void BlockBuildUtils::writePortNameSettersForAtomBlocks(std::ofstream& o, SpecStash& ss, const InstanceList& il){
    for (auto& inst: il){
        auto& instOf = ss[inst.path];
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            auto& atom = std::get<AtomSpec>(instOf);
            writePortNameSetters(o, inst.name, atom.outputs);
        }
    }
}

void BlockBuildUtils::writeConnections(std::ofstream& o, const ConnectionList& cl){
    for (auto& conn: cl){
        std::string fromName = BlockBuildUtils::qualifiedPortname(conn.from, conn.from_port);
        std::string toName = BlockBuildUtils::qualifiedPortname(conn.to, conn.to_port);
        o << "\t\tconnect(&" << fromName << ", &" << toName << ", ____name + \"" << toName << "\");\n";
    }
}

void BlockBuildUtils::writeConstValueSetters(std::ofstream& o, const ConstantList& cl){
    for (auto& cnst: cl){
        std::string toName = BlockBuildUtils::qualifiedPortname(cnst.to, cnst.to_port);
        o << "\t\tsetConstValue(&" << toName << ", " << cnst.value << ", ____name + \"" << toName << "\");\n";
    }
}

std::string BlockBuildUtils::qualifiedPortname(const std::string& instance, const std::string& port){
    return (instance == "" ? port : instance + '.' + port);
}
