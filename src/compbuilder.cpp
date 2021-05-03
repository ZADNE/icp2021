/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "compbuilder.h"

#include <filesystem>
#include <algorithm>

#include "blockbuildutils.h"
#include "speccache.h"

namespace fs = std::filesystem;

CompBuilder::CompBuilder(const std::string& libPath):
    m_libPath(libPath){

}

bool CompBuilder::buildComp(const std::string& headerPath, const CompSpec& comp,
                             std::set<std::string>* toBeBuilt){
    std::ofstream o{m_libPath + headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Macro guard
    BlockBuildUtils::writeGuardStart(o, comp.name);
    //Includes
    size_t undive = std::count(headerPath.begin(), headerPath.end(), '/') - 1;
    BlockBuildUtils::writeInstanceIncludes(o, comp.instances, undive);
    //Templates
    //BlockBuildUtils::writeTemplates(o, comp.inputs, comp.outputs);
    //Class
    o << "class " << comp.name << ": public ____Signaller{\n";
    o << "public:\n";
    //Constructor
    o << '\t' << comp.name << "(const std::string& ____name):\n";
    //Instance initializer list
    for (auto& inst: comp.instances){
        const AnySpec& instOf = SpecCache::fetchAny(inst.path);
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            o << "\t\t" << inst.name << "(&____changed),\n";
        } else if (std::holds_alternative<CompSpec>(instOf)){
            //Instance of composite block
            o << "\t\t" << inst.name << "(____name + \"" << inst.name << ".\"),\n";
        } else throw;
    }
    o << "\t\t____changed(false)\n";
    o << "\t{\n";
    //Name input ports of this block
    BlockBuildUtils::writePortNameSetters(o, "", comp.inputs);
    //Name output ports of this block
    BlockBuildUtils::writePortNameSetters(o, "", comp.outputs);
    //Name output ports of atomic blocks directly inside this composite block
    BlockBuildUtils::writePortNameSettersForAtomBlocks(o, comp.instances);
    //Connect ports
    BlockBuildUtils::writeConnections(o, comp.connections);
    //Set constant values
    BlockBuildUtils::writeConstValueSetters(o, comp.constants);
    o << "\t}\n\n";
    //Main function loop
    o << "\tvoid func() {\n";
    o << "\t\tdo {\n";
    o << "\t\t\t____changed = false;\n";
    for (auto& inst: comp.instances){
        o << "\t\t\t" << inst.name << ".func();\n";
    }
    o << "\t\t} while(____changed);\n";
    o << "\t}\n\n";
    //Instances
    for (auto& inst: comp.instances){
        const AnySpec& instOf = SpecCache::fetchAny(inst.path);
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            auto& atom = std::get<AtomSpec>(instOf);
            o << "\t" << atom.name << ' ' << inst.name << ";\n";
        } else if (std::holds_alternative<CompSpec>(instOf)){
            //Instance of composite block
            auto& comp = std::get<CompSpec>(instOf);
            o << "\t" << comp.name << ' ' << inst.name << ";\n";
        } else throw;
    }
    o << '\n';
    //Input ports
    for (auto& input: comp.inputs){
        o << "\t____RepeaterPort<" << input.type <<  "> " << input.name << ";\n";
    }
    //Output ports
    for (auto& output: comp.outputs){
        o << "\t____RepeaterPort<" << output.type <<  "> " << output.name << ";\n";
    }
    //Protected
    o << "protected:\n";
    o << "\tbool ____changed;\n";
    //End class
    o << "};\n";
    o << "#endif" << std::endl;
    o.close();

    //Gather unbuilt blocks
    if (!toBeBuilt) return true;
    std::error_code ec;
    for (auto& inst: comp.instances){
        auto absPath = m_libPath + inst.path;
        auto xmlEditTime = fs::last_write_time(absPath, ec);
        if (xmlEditTime > fs::last_write_time(absPath + ".hpp", ec)){
            toBeBuilt->insert(inst.path);
        }
    }
    return true;
}
