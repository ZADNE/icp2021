/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "blockcompiler.h"

#include <fstream>
#include <algorithm>
#include <set>

#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "xmlutils.h"
#include "specstash.h"

//Helper functions
void writeTemplates(std::ofstream& o, const SlotList& inputs, const SlotList& outputs);
void writeGuardStartAndLib(std::ofstream& o, std::string blockName);
void writeInstanceIncludes(std::ofstream& o, const InstanceList& il);
std::string qualifiedPortname(const std::string& instance, const std::string& port);

BlockCompiler::BlockCompiler(){

}

void BlockCompiler::setCppCompiler(const std::string& compiler){
    m_cppCompiler = compiler + ' ';
}

bool BlockCompiler::buildAtom(const std::string& atomPath){
    AtomSpec atom;
    if (!readAtom(atomPath, atom)) return false;
    std::string headerPath = atomPath + ".hpp";
    return buildAtom(headerPath, atom);
}

bool BlockCompiler::readAtom(const std::string& atomPath, AtomSpec& atom){
    return ::readAtom(atomPath, atom);
}

bool BlockCompiler::writeAtom(const std::string& atomPath, const AtomSpec& atom){
    return ::writeAtom(atomPath, atom);
}

bool BlockCompiler::buildComp(const std::string& compPath){
    CompSpec comp;
    if (!readComp(compPath, comp)) return false;
    std::string headerPath = compPath + ".hpp";
    try {
        return buildComp(headerPath, comp);
    }  catch (...) {
        return false;
    }
}

bool BlockCompiler::readComp(const std::string& compPath, CompSpec& comp){
    return ::readComp(compPath, comp);
}

bool BlockCompiler::writeComp(const std::string& compPath, const CompSpec& comp){
    return ::writeComp(compPath, comp);
}

BlockCompiler& BlockCompiler::bc(){
    static BlockCompiler bc;
    return bc;
}

bool BlockCompiler::buildAtom(const std::string& headerPath, const AtomSpec& atom){
    std::ofstream o{headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    //Macro guard
    writeGuardStartAndLib(o, atom.name);
    //Templates
    writeTemplates(o, atom.inputs, atom.outputs);
    //Class
    o << "class " << atom.name << "{\n";
    o << "public:\n";
    //Constructor
    o << '\t' << atom.name << "(bool* ic):\n\t";
    for (int i = 0; i < (int)atom.inputs.size() - 1; ++i){
        o << atom.inputs[i].name << "(ic), ";
    }
    if (atom.inputs.size() >= 1){//Last without the comma...
        o << atom.inputs.back().name << "(ic)";
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
    o << "};\n";
    o << "#endif" << std::endl;
    o.close();

    //std::string command = m_cppCompiler + m_cppFlags + headerPath;
    //qDebug() << std::system(command.c_str());
    return true;
}

bool BlockCompiler::buildComp(const std::string& headerPath, const CompSpec& comp){
    std::ofstream o{headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create the file :-/
    SpecStash ss;
    //Macro guard
    writeGuardStartAndLib(o, comp.name);
    //Includes
    writeInstanceIncludes(o, comp.instances);
    //Templates
    writeTemplates(o, comp.inputs, comp.outputs);
    //Class
    o << "class " << comp.name << ": public ____Signaller{\n";
    o << "public:\n";
    //Constructor
    o << '\t' << comp.name << "(const std::string& ____name):\n";
    for (auto& inst: comp.instances){
        if (std::holds_alternative<AtomSpec>(ss[inst.path])){
            //Instance of atomic block
            o << "\t\t" << inst.name << "(&____changed)\n";
        } else {
            //Instance of composite block
            o << "\t\t" << inst.name << "(\"" << inst.name << ".\")\n";
        }

    }
    o << "\t{";
    //Name output ports
    for (auto& out: comp.outputs){
        o << "\t\tsetName(&" << out.name << ", ____name + \"" << out.name << "\");\n";
    }
    //Connect ports
    for (auto& conn: comp.connections){
        std::string fromName = qualifiedPortname(conn.from, conn.from_port);
        std::string toName = qualifiedPortname(conn.to, conn.to_port);
        o << "\t\tconnect(&" << fromName << ", &" << toName << ", ____name + \"" << toName << "\");\n";
    }
    o << '\n';
    //Name output ports of atomic blocks
    for (auto& inst: comp.instances){
        auto& instOf = ss[inst.path];
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            auto& atom = std::get<AtomSpec>(instOf);
            for (auto& out: atom.outputs){
                std::string name = qualifiedPortname(inst.name, out.name);
                o << "\t\tsetName(&" << name << ", ____name + \"" << name << "\");\n";
            }
        }
    }
    o << '\n';
    //Set const values
    for (auto& cnst: comp.constants){
        std::string toName = qualifiedPortname(cnst.to, cnst.to_port);
        o << "\t\tsetConstValue(&" << toName << ", " << cnst.value << ", ____name + \"" << toName << "\");\n";
    }
    o << "\t}\n\n";
    //Function
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
        auto& instOf = ss[inst.path];
        if (std::holds_alternative<AtomSpec>(instOf)){
            //Instance of atomic block
            auto& atom = std::get<AtomSpec>(instOf);
            o << "\t" << atom.name << ' ' << inst.name << ";\n";
        } else {
            //Instance of composite block
            auto& comp = std::get<CompSpec>(instOf);
            o << "\t" << comp.name << ' ' << inst.name << ";\n";
        }

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
    o << "};";
    o << "#endif" << std::endl;
    o.close();
    return true;
}

bool BlockCompiler::openLibrary(const char* path){
    m_libPath = path;
    return initLibrary();
}

bool BlockCompiler::initLibrary(){
    auto headerPath = m_libPath + "/library.hpp";
    std::ifstream i{headerPath};
    if (i.good()) {
        //Header already exists
        i.close();
        return true;
    }
    std::ofstream o{headerPath, std::ofstream::out};
    if (!o.good()) return false;//Cannot create header :-/

    static const char header[] = R"&|&(
/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef ____LIBRARY_H
#define ____LIBRARY_H
#include <queue>
#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <optional>

/*class ____Block{
public:
    virtual void func() = 0;
};*/

template<typename T> class ____ReceiverPort {
public:
    virtual void pushValue(const T& value) = 0;
    virtual void setConstValue(const T& value, const std::string& myName) = 0;
};

template<typename T> class ____InPort: public ____ReceiverPort<T> {
template<typename> friend class ____OutPort;
friend class ____Signaller;
public:

    ____InPort(bool* changed): p_changed(changed){}

    bool hasValue() const {
        if (p_constValue){
            return true;
        } else {
            return static_cast<bool>(p_queue.size());
        }
    }

    T getValue(){
        if (p_constValue){
            return p_constValue.value();
        } else {
            *p_changed = true;
            T tmp = p_queue.front();
            p_queue.pop();
            return tmp;
        }
    }

protected:
    void pushValue(const T& value) override {
        if (p_queue.empty() && !p_constValue){
            *p_changed = true;
        }
        p_queue.push(value);
    }

    void setConstValue(const T& value, const std::string& myName) override {
        std::cout << "$CONST$ [" << value << "] -> " << myName << std::endl;
        p_constValue = std::make_optional(value);
    }

    std::queue<T> p_queue;
    std::optional<T> p_constValue;
    bool* p_changed;
};

template<typename T> class ____OutPort{
friend class ____Signaller;
public:

    void putValue(const T& value){
        for (auto& cnct: p_connections){
            std::cout << p_from << " [" << value << "] -> " << cnct.second << std::endl;
            cnct.first->pushValue(value);
        }
    }

protected:
    void setName(const std::string& name) {
        p_from = name;
    }

    void connect(____ReceiverPort<T>* port, const std::string& receiverName) {
        p_connections.emplace_back(port, receiverName);
    }

protected:
    std::vector<std::pair<____ReceiverPort<T>*, std::string>> p_connections;
    std::string p_from;
};

template<typename T> class ____RepeaterPort: public ____ReceiverPort<T>, public ____OutPort<T>{
friend class ____Signaller;
protected:

    void pushValue(const T& value) override {
        this->putValue(value);
    }

    void setConstValue(const T& value, const std::string& myName) override {
        std::cout << "$CONST$ [" << value << "] -> " << myName << std::endl;
        for (auto& cnct: ____OutPort<T>::p_connections){
            cnct.first->setConstValue(value, cnct.second);
        }
    }
};

class ____Signaller{
public:
    template<typename T> void connect(
        ____OutPort<T>* fromPort, ____ReceiverPort<T>* toPort, const std::string& receiverName){
        fromPort->connect(toPort, receiverName);
    }

    template<typename T> void setName(____OutPort<T>* port, const std::string& name){
        port->setName(name);
    }

    template<typename T> void setConstValue(____ReceiverPort<T>* port, const T& value, const std::string& myName){
        port->setConstValue(value, myName);
    }
};

#endif // !____LIBRARY_H
        )&|&";

    o.write(header, sizeof(header) - 1);
    o.close();
    //std::string command = m_cppCompiler + m_cppFlags + headerPath;
    //qDebug() << std::system(command.c_str());
    return true;
}

void writeTemplates(std::ofstream& o, const SlotList& inputs, const SlotList& outputs){
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


void writeGuardStartAndLib(std::ofstream& o, std::string blockName){
    //Guardify name
    std::transform(blockName.begin(), blockName.end(), blockName.begin(), std::toupper);
    blockName.append("_H");
    //Write
    o << "#ifndef " << blockName << '\n';
    o << "#define " << blockName << '\n';
    o << "#include \"library.hpp\"\n";
}

void writeInstanceIncludes(std::ofstream& o, const InstanceList& il){
    for (auto& inst: il){
        o << "#include \"" << inst.path << ".hpp\n";
    }
}

std::string qualifiedPortname(const std::string& instance, const std::string& port){
    return (instance == "" ? port : instance + '.' + port);
}
