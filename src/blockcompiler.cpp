/***
 * \author Tomáš Dubský (xdubsk08)
 * */
#include "blockcompiler.h"

#include <fstream>
#include <algorithm>

#include "rapidxml/rapidxml_utils.hpp"

#include <QDebug>

//Helper functions
void toGuard(std::string& name);

BlockCompiler::BlockCompiler(){

}

void BlockCompiler::setCppCompiler(std::string compiler){
    m_cppCompiler = compiler + ' ';
}

bool BlockCompiler::buildAtom(const std::string& atomPath){
    AtomSpec atom;
    try {
        if (!readAtom(atomPath, atom)) return false;
    }  catch (rapidxml::parse_error) {
        return false;
    }  catch (std::runtime_error) {
        return false;
    }
    std::string headerPath = atomPath + ".h";
    return buildAtom(headerPath, atom);
}

bool BlockCompiler::readAtom(const std::string& atomPath, AtomSpec& atom){
    std::ifstream is{atomPath};
    rapidxml::file<char> file(is);
    rapidxml::xml_document<char> doc;
    doc.parse<0>(file.data());
    //Atom
    auto* node = doc.first_node();
    if (!node) return false;
    if (strcmp(node->name(), "ATOM") != 0) return false;
    //Name
    auto* attr = node->first_attribute();
    if (!attr) return false;
    if (strcmp(attr->name(), "NAME") != 0) return false;
    atom.name = attr->value();
    //Input ports
    if (!(node = node->first_node())) return false;
    if (strcmp(node->name(), "INPUT_PORTS") != 0) return false;
    if (!extractPorts(node->first_node(), atom.inputs)) return false;
    //Output ports
    if (!(node = node->next_sibling())) return false;
    if (strcmp(node->name(), "OUTPUT_PORTS") != 0) return false;
    if (!extractPorts(node->first_node(), atom.outputs)) return false;
    //Function body
    if (!(node = node->next_sibling())) return false;
    if (strcmp(node->name(), "FUNCTION_BODY") != 0) return false;
    atom.body = node->value();
    //State variables
    if (!(node = node->next_sibling())) return false;
    if (strcmp(node->name(), "STATE_VARIABLES") != 0) return false;
    atom.stateVars = node->value();
    return true;
}

bool BlockCompiler::extractPorts(rapidxml::xml_node<char>* node, SlotList& sl){
    while (node){
        sl.emplace_back(node->value(), node->name());
        node = node->next_sibling();
    }
    return true;
}

BlockCompiler& BlockCompiler::bc(){
    static BlockCompiler bc;
    return bc;
}

bool BlockCompiler::buildAtom(const std::string& headerPath, const AtomSpec& atom){
    std::ofstream o{headerPath, std::ofstream::trunc};
    if (!o.good()) return false;//Cannot create file :-/
    std::string guard = atom.name;
    toGuard(guard);
    //Macro guard
    o << "#ifndef " << guard << std::endl;
    o << "#define " << guard << std::endl;
    o << "#include \"block.h\"\n";
    //Class
    o << "class " << atom.name << ": public ____Block{\n";
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
    o << "void func() override {\n";
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
    o << "#endif //!" << guard << std::endl;
    o.close();

    std::string command = m_cppCompiler + m_cppFlags + headerPath;
    qDebug() << std::system(command.c_str());
    return true;
}

bool BlockCompiler::openLibrary(const char* path){
    m_libPath = path;
    return checkLibraryHeaderHeader();
}

bool BlockCompiler::checkLibraryHeaderHeader(){
    auto headerPath = m_libPath + "/block.h";
    std::ifstream i{headerPath};
    if (i.good()) {
        //Header already exists
        i.close();
        return true;
    }
    std::ofstream o{headerPath, std::ofstream::out};
    if (!o.good()) return false;//Cannot header :-/

    static const char header[] =
        "/***\n"
        " * \\author Tomáš Dubský (xdubsk08)\n"
        " * */\n"
        "#ifndef ____BLOCK_H\n"
        "#define ____BLOCK_H\n"
        "#include <queue>\n"
        "#include <vector>\n"
        "#include <utility>\n"
        "#include <string>\n"
        "#include <iostream>\n"
        "#include <optional>\n"
        "\n"
        "class ____Block{\n"
        "public:\n"
        "    virtual void func() = 0;\n"
        "};\n"
        "\n"
        "template<typename T> class ____InPort {\n"
        "template<typename> friend class ____OutPort;\n"
        "friend class ____Appl;\n"
        "public:\n"
        "\n"
        "    ____InPort(bool* changed): p_changed(changed){}\n"
        "\n"
        "    bool hasValue() const {\n"
        "        if (p_constValue){\n"
        "            return true;\n"
        "        } else {\n"
        "            return static_cast<bool>(p_queue.size());\n"
        "        }\n"
        "    }\n"
        "\n"
        "    T getValue(){\n"
        "        if (p_constValue){\n"
        "            return p_constValue.value();\n"
        "        } else {\n"
        "            *p_changed = true;\n"
        "            T tmp = p_queue.front();\n"
        "            p_queue.pop();\n"
        "            return tmp;\n"
        "        }\n"
        "    }\n"
        "\n"
        "protected:\n"
        "    void putValue(const T& value){\n"
        "        if (p_queue.empty()){\n"
        "            *p_changed = true;\n"
        "        }\n"
        "        p_queue.push(value);\n"
        "    }\n"
        "\n"
        "    void setConstValue(const T& value){\n"
        "        p_constValue = std::make_optional(value);\n"
        "    }\n"
        "\n"
        "    std::queue<T> p_queue;\n"
        "    std::optional<T> p_constValue;\n"
        "    bool* p_changed;\n"
        "};\n"
        "\n"
        "template<typename T> class ____OutPort{\n"
        "friend class ____Appl;\n"
        "public:\n"
        "\n"
        "    void putValue(const T& value){\n"
        "        for (auto& cnct: p_connections){\n"
        "            std::cout << p_from << \" -> [\" << value << \"] \" << cnct.second << std::endl;\n"
        "            cnct.first->putValue(value);\n"
        "        }\n"
        "    }\n"
        "\n"
        "protected:\n"
        "    void setSenderName(const std::string& name){\n"
        "        p_from = name;\n"
        "    }\n"
        "\n"
        "    void connect(____InPort<T>* port, const std::string& receiver){\n"
        "        p_connections.emplace_back(port, receiver);\n"
        "    }\n"
        "\n"
        "    std::vector<std::pair<____InPort<T>*, std::string>> p_connections;\n"
        "    std::string p_from;\n"
        "};\n"
        "#endif // !____BLOCK_H\n";

    o.write(header, sizeof(header) - 1);
    o.close();
    std::string command = m_cppCompiler + m_cppFlags + headerPath;
    qDebug() << std::system(command.c_str());
    return true;
}

void toGuard(std::string& name){
    std::transform(name.begin(), name.end(), name.begin(), std::toupper);
    name.append("_H");
}
