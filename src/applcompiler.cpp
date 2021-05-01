/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "applcompiler.h"

#include "blockbuildutils.h"
#include "speccache.h"

ApplCompiler::ApplCompiler(const std::string& libPath):
    m_libPath(libPath){

}

bool ApplCompiler::buildAppl(const std::string& filePath, const ApplSpec& appl){
    std::ofstream o{filePath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Includes
    o << "#include \"library.hpp\"\n\n";
    BlockBuildUtils::writeInstanceIncludes(o, appl.instances);
    //Class
    o << "class " << appl.name << ": public ____Signaller{\n";
    o << "public:\n";
    o << "\tvoid run(){\n";
    o << "\t\tbool ____changed;\n";
    o << "\t\tstd::string ____name;\n";
    //Instances
    for (auto& inst: appl.instances){
        const AnySpec& instOf = SpecCache::fetchAny(inst.path);
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            auto& atom = std::get<AtomSpec>(instOf);
            o << "\t\t" << atom.name << ' ' << inst.name << "{&____changed};\n";
        } else if (std::holds_alternative<CompSpec>(instOf)) {
            //Instance of composite block
            auto& comp = std::get<CompSpec>(instOf);
            o << "\t\t" << comp.name << ' ' << inst.name << "{\"" << inst.name << ".\"};\n";
        } else {
            throw;
        }
    }
    //Name output ports of atomic blocks directly inside this application
    BlockBuildUtils::writePortNameSettersForAtomBlocks(o, appl.instances);
    //Connect ports
    BlockBuildUtils::writeConnections(o, appl.connections);
    //Set constant values
    BlockBuildUtils::writeConstValueSetters(o, appl.constants);

    o << '\n';
    //Main loop
    o << "\t\tdo {\n";
    o << "\t\t\t____changed = false;\n";
    for (auto& inst: appl.instances){
        o << "\t\t\t" << inst.name << ".func();\n";
    }
    o << "\t\t} while(____changed);\n";
    o << "\t}\n";
    o << "};\n";
    o << '\n';
    //C++ main
    o << "int main(){\n";
    o << '\t' << appl.name << ' ' << "____appl;\n";
    o << "\t____appl.run();\n";
    o << "\treturn 0;\n";
    o << "}\n";

    o.close();
    return true;
}

