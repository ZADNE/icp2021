/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "applbuilder.h"

#include <filesystem>

#include "blockbuildutils.h"
#include "speccache.h"

namespace fs = std::filesystem;

ApplBuilder::ApplBuilder(const std::string& libPath):
    m_libPath(libPath){

}

bool ApplBuilder::buildAppl(const std::string& filePath, const ApplSpec& appl,
                             std::set<std::string>& toBeBuilt){
    std::ofstream o{m_libPath + filePath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Includes
    o << "#include \"library.hpp\"\n\n";
    size_t undive = std::count(filePath.begin(), filePath.end(), '/') - 1;
    BlockBuildUtils::writeInstanceIncludes(o, appl.instances, undive);
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
    if (appl.maxSteps != 0){
        o << "\t\tsize_t ____c = 0u;\n";
        o << "\t\tconst size_t ____maxSteps = " << std::to_string(appl.maxSteps) << ";\n";
    }
    o << "\t\tdo {\n";
    o << "\t\t\t____changed = false;\n";
    for (auto& inst: appl.instances){
        o << "\t\t\t" << inst.name << ".func();\n";
    }
    //End condition
    o << "\t\t} while(____changed" << (appl.maxSteps != 0 ? " && (++____c < ____maxSteps)" : "") << ");\n";
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

    //Gather unbuilt blocks
    std::error_code ec;
    for (auto& inst: appl.instances){
        auto absPath = m_libPath + inst.path;
        auto xmlEditTime = fs::last_write_time(absPath, ec);
        if (xmlEditTime > fs::last_write_time(absPath + ".hpp", ec) || ec){
            toBeBuilt.insert(inst.path);
        }
    }

    return true;
}

