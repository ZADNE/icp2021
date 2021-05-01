/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "compcompiler.h"

#include "blockbuildutils.h"
#include "speccache.h"

CompCompiler::CompCompiler(const std::string& libPath):
    m_libPath(libPath){

}

bool CompCompiler::buildComp(const std::string& headerPath, const CompSpec& comp){
    std::ofstream o{headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Macro guard
    BlockBuildUtils::writeGuardStartAndLib(o, comp.name);
    //Includes
    BlockBuildUtils::writeInstanceIncludes(o, comp.instances);
    //Templates
    BlockBuildUtils::writeTemplates(o, comp.inputs, comp.outputs);
    //Class
    o << "class " << comp.name << ": public ____Signaller{\n";
    o << "public:\n";
    //Constructor
    o << '\t' << comp.name << "(const std::string& ____name):\n";
    for (auto& inst: comp.instances){
        const AnySpec& instOf = SpecCache::fetchAny(inst.path);
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            o << "\t\t" << inst.name << "(&____changed)\n";
        } else if (std::holds_alternative<CompSpec>(instOf)){
            //Instance of composite block
            o << "\t\t" << inst.name << "(\"" << inst.name << ".\")\n";
        } else throw;
    }
    o << "\t{\n";
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
    return true;
}
