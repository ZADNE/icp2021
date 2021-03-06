/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "atombuilder.h"

#include "blockbuildutils.h"

AtomBuilder::AtomBuilder(const std::string& libPath):
    m_libPath(libPath){

}

bool AtomBuilder::buildAtom(const std::string& headerPath, const AtomSpec& atom){
    std::ofstream o{m_libPath + headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Macro guard
    BlockBuildUtils::writeGuardStart(o, atom.name);
    //Templates
    //BlockBuildUtils::writeTemplates(o, atom.inputs, atom.outputs);
    //Class
    o << "class " << atom.name << "{\n";
    o << "public:\n";
    //Constructor
    o << '\t' << atom.name << "(bool* ____changed)";
    if (!atom.inputs.empty()){
        //Inputs initializer list
        o << ":\n\t";
        for (int i = 0; i < (int)atom.inputs.size() - 1; ++i){
            o << atom.inputs[i].name << "(____changed), ";
        }
        if (atom.inputs.size() >= 1){//Last without the comma...
            o << atom.inputs.back().name << "(____changed)";
        }
    } else {
        o << "\n\t";
    }
    o << "{}\n\t";
    //Func
    o << "void func() {\n";
    o << atom.body;
    o << "\n/**/\n\t}\n";
    //Input ports
    for (auto& input: atom.inputs){
        o << "\t____InPort<" << input.type << "> " << input.name << ";\n";
    }
    //Output ports
    for (auto& output: atom.outputs){
        o << "\t____OutPort<" << output.type << "> " << output.name << ";\n";
    }
    //State variables
    o << "private:\n\t";
    o << atom.stateVars << '\n';
    //End class
    o << "};\n";
    o << "#endif" << std::endl;
    o.close();
    return true;
}
