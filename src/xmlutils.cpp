/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "xmlutils.h"

#include <fstream>
#include <filesystem>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"


AnySpec XMLUtils::readAny(const std::string& filePath){
    AnySpec spec;//Monostate
    auto ext = std::filesystem::path(filePath).extension().string();
    if (ext == ".atom"){
        AtomSpec atom;
        if (XMLUtils::readAtom(filePath, atom)){
            spec = atom;
        }
    } else if (ext == ".comp"){
        CompSpec comp;
        if (XMLUtils::readComp(filePath, comp)){
            spec = comp;
        }
    } else if (ext == ".appl"){
        ApplSpec appl;
        if (XMLUtils::readAppl(filePath, appl)){
            spec = appl;
        }
    }
    return spec;
}

bool XMLUtils::writeAny(const std::string& filePath, const AnySpec& any){
    if (std::holds_alternative<AtomSpec>(any)){
        return writeAtom(filePath, std::get<AtomSpec>(any));
    } else if (std::holds_alternative<CompSpec>(any)){
        return writeComp(filePath, std::get<CompSpec>(any));
    } else if (std::holds_alternative<ApplSpec>(any)){
        return writeAppl(filePath, std::get<ApplSpec>(any));
    } else {
        return false;
    }
}


bool XMLUtils::readAtom(const std::string& atomPath, AtomSpec& atom){
    try {
        std::ifstream is{atomPath};
        rapidxml::file<char> file(is);
        rapidxml::xml_document<char> doc;
        doc.parse<0>(file.data());
        //Atom
        auto* node = doc.first_node();
        if (!node || strcmp(node->name(), "ATOM") != 0) return false;
        //Name
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "NAME") != 0) return false;
        atom.name = attr->value();
        //Input ports
        if (!(node = node->first_node()) || strcmp(node->name(), "INPUT_PORTS") != 0) return false;
        extractPorts(node->first_node(), atom.inputs);
        //Output ports
        if (!(node = node->next_sibling()) || strcmp(node->name(), "OUTPUT_PORTS") != 0) return false;
        extractPorts(node->first_node(), atom.outputs);
        //Function body
        if (!(node = node->next_sibling()) || strcmp(node->name(), "FUNCTION_BODY") != 0) return false;
        atom.body = node->value();
        //State variables
        if (!(node = node->next_sibling()) || strcmp(node->name(), "STATE_VARIABLES") != 0) return false;
        atom.stateVars = node->value();
        return true;
    }  catch (...) {
        return false;
    }
}

bool XMLUtils::readComp(const std::string& compPath, CompSpec& comp){
    try {
        std::ifstream is{compPath};
        rapidxml::file<char> file(is);
        rapidxml::xml_document<char> doc;
        doc.parse<0>(file.data());
        //Comp
        auto* node = doc.first_node();
        if (!node || strcmp(node->name(), "COMP") != 0) return false;
        //Name
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "NAME") != 0) return false;
        comp.name = attr->value();
        //Input ports
        if (!(node = node->first_node()) || strcmp(node->name(), "INPUT_PORTS") != 0) return false;
        extractPorts(node->first_node(), comp.inputs);
        //Output ports
        if (!(node = node->next_sibling()) || strcmp(node->name(), "OUTPUT_PORTS") != 0) return false;
        extractPorts(node->first_node(), comp.outputs);
        //Instances
        if (!(node = node->next_sibling()) || strcmp(node->name(), "INSTANCES") != 0) return false;
        if (!extractInstances(node->first_node(), comp.instances)) return false;
        //Connections
        if (!(node = node->next_sibling()) || strcmp(node->name(), "CONNECTIONS") != 0) return false;
        if (!extractConnections(node->first_node(), comp.connections)) return false;
        //Constants
        if (!(node = node->next_sibling()) || strcmp(node->name(), "CONSTANTS") != 0) return false;
        if (!extractContants(node->first_node(), comp.constants)) return false;
        return true;
    }  catch (...) {
        return false;
    }
}

bool XMLUtils::readAppl(const std::string& applPath, ApplSpec& appl){
    try {
        std::ifstream is{applPath};
        rapidxml::file<char> file(is);
        rapidxml::xml_document<char> doc;
        doc.parse<0>(file.data());
        //Comp
        auto* node = doc.first_node();
        if (!node || strcmp(node->name(), "APPL") != 0) return false;
        //Name
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "NAME") != 0) return false;
        appl.name = attr->value();
        //Instances
        if (!(node = node->first_node()) || strcmp(node->name(), "INSTANCES") != 0) return false;
        if (!extractInstances(node->first_node(), appl.instances)) return false;
        //Connections
        if (!(node = node->next_sibling()) || strcmp(node->name(), "CONNECTIONS") != 0) return false;
        if (!extractConnections(node->first_node(), appl.connections)) return false;
        //Constants
        if (!(node = node->next_sibling()) || strcmp(node->name(), "CONSTANTS") != 0) return false;
        if (!extractContants(node->first_node(), appl.constants)) return false;
        return true;
    }  catch (...) {
        return false;
    }
}

bool XMLUtils::writeAtom(const std::string& atomPath, const AtomSpec& atom){
    std::ofstream os{atomPath};
    if (os.fail()) return false;
    rapidxml::xml_document<char> doc;
    xml_node* node;
    //Atom
    auto* atomNode = doc.allocate_node(rapidxml::node_element, "ATOM");
    doc.append_node(atomNode);
    //Name
    atomNode->append_attribute(doc.allocate_attribute("NAME", atom.name.c_str()));
    //Inputs
    node = doc.allocate_node(rapidxml::node_element, "INPUT_PORTS");
    atomNode->append_node(node);
    insertPorts(&doc, node, atom.inputs);
    //Outputs
    node = doc.allocate_node(rapidxml::node_element, "OUTPUT_PORTS");
    atomNode->append_node(node);
    insertPorts(&doc, node, atom.outputs);
    //Function body
    atomNode->append_node(doc.allocate_node(rapidxml::node_element, "FUNCTION_BODY", atom.body.c_str()));
    //State variables
    atomNode->append_node(doc.allocate_node(rapidxml::node_element, "STATE_VARIABLES", atom.stateVars.c_str()));
    //Write to file
    os << doc;
    os.close();
    return true;
}

bool XMLUtils::writeComp(const std::string& compPath, const CompSpec& comp){
    std::ofstream os{compPath};
    if (os.fail()) return false;
    rapidxml::xml_document<char> doc;
    xml_node* node;
    //Atom
    auto* atomNode = doc.allocate_node(rapidxml::node_element, "COMP");
    doc.append_node(atomNode);
    //Name
    atomNode->append_attribute(doc.allocate_attribute("NAME", comp.name.c_str()));
    //Inputs
    node = doc.allocate_node(rapidxml::node_element, "INPUT_PORTS");
    atomNode->append_node(node);
    insertPorts(&doc, node, comp.inputs);
    //Outputs
    node = doc.allocate_node(rapidxml::node_element, "OUTPUT_PORTS");
    atomNode->append_node(node);
    insertPorts(&doc, node, comp.outputs);
    //Instances
    node = doc.allocate_node(rapidxml::node_element, "INSTANCES");
    atomNode->append_node(node);
    insertInstances(&doc, node, comp.instances);
    //Connections
    node = doc.allocate_node(rapidxml::node_element, "CONNECTIONS");
    atomNode->append_node(node);
    insertConnections(&doc, node, comp.connections);
    //Constants
    node = doc.allocate_node(rapidxml::node_element, "CONSTANTS");
    atomNode->append_node(node);
    insertConstants(&doc, node, comp.constants);
    //Write to file
    os << doc;
    os.close();
    return true;
}


bool XMLUtils::writeAppl(const std::string& applPath, const ApplSpec& appl){
    std::ofstream os{applPath};
    if (os.fail()) return false;
    rapidxml::xml_document<char> doc;
    xml_node* node;
    //Atom
    auto* atomNode = doc.allocate_node(rapidxml::node_element, "APPL");
    doc.append_node(atomNode);
    //Name
    atomNode->append_attribute(doc.allocate_attribute("NAME", appl.name.c_str()));
    //Instances
    node = doc.allocate_node(rapidxml::node_element, "INSTANCES");
    atomNode->append_node(node);
    insertInstances(&doc, node, appl.instances);
    //Connections
    node = doc.allocate_node(rapidxml::node_element, "CONNECTIONS");
    atomNode->append_node(node);
    insertConnections(&doc, node, appl.connections);
    //Constants
    node = doc.allocate_node(rapidxml::node_element, "CONSTANTS");
    atomNode->append_node(node);
    insertConstants(&doc, node, appl.constants);
    //Write to file
    os << doc;
    os.close();
    return true;
}

XMLUtils::XMLUtils(){

}

void XMLUtils::extractPorts(xml_node* node, SlotList& sl){
    while (node){
        bool templ = false;
        auto* attr = node->first_attribute();
        if (attr
            && strcmp(attr->name(), "TEMPLATE") == 0
            && strcmp(attr->value(), "TRUE") == 0){
            templ = true;
        }
        sl.emplace_back(templ, node->value(), node->name());
        node = node->next_sibling();
    }
}

bool XMLUtils::extractInstances(xml_node* node, InstanceList& il){
    while (node){
        //Position
        int x, y;
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "X") != 0) return false;
        x = std::stoi(attr->value());
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "Y") != 0) return false;
        y = std::stoi(attr->value());
        //Emplace the instance
        il.emplace_back(node->name(), node->value(), x, y);
        node = node->next_sibling();
    }
    return true;
}

bool XMLUtils::extractConnections(xml_node* node, ConnectionList& cl){
    ConnectionSpec spec;
    while (node){
        if (strcmp(node->name(), "CONNECTION") != 0) return false;
        //From
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "FROM") != 0) return false;
        spec.from = attr->value();
        //From port
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "FROM_PORT") != 0) return false;
        spec.from_port = attr->value();
        //To
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "TO") != 0) return false;
        spec.to = attr->value();
        //To port
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "TO_PORT") != 0) return false;
        spec.to_port = attr->value();
        //Add the connection
        cl.push_back(spec);
        node = node->next_sibling();
    }
    return true;
}

bool XMLUtils::extractContants(xml_node* node, ConstantList& cl){
    ConstantSpec spec;
    while (node){
        if (strcmp(node->name(), "CONSTANT") != 0) return false;
        //Value
        auto* attr = node->first_attribute();
        if (!attr || strcmp(attr->name(), "VALUE") != 0) return false;
        spec.value = attr->value();
        //To
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "TO") != 0) return false;
        spec.to = attr->value();
        //To port
        attr = attr->next_attribute();
        if (!attr || strcmp(attr->name(), "TO_PORT") != 0) return false;
        spec.to_port = attr->value();
        //Add the connection
        cl.push_back(spec);
        node = node->next_sibling();
    }
    return true;
}

void XMLUtils::insertPorts(xml_doc* doc, xml_node* node, const SlotList& sl){
    for(auto& slot: sl){
        auto* slotNode = doc->allocate_node(rapidxml::node_element, slot.name.c_str(), slot.type.c_str());
        slotNode->append_attribute(doc->allocate_attribute("TEMPLATE", slot.templ ? "TRUE": "FALSE"));
        node->append_node(slotNode);
    }
}

void XMLUtils::insertInstances(xml_doc* doc, xml_node* node, const InstanceList& il){
    for(auto& inst: il){
        auto* instNode = doc->allocate_node(rapidxml::node_element, inst.name.c_str(), inst.path.c_str());
        instNode->append_attribute(doc->allocate_attribute(
            "X", doc->allocate_string(std::to_string(inst.x).c_str())));
        instNode->append_attribute(doc->allocate_attribute(
            "Y", doc->allocate_string(std::to_string(inst.y).c_str())));
        node->append_node(instNode);
    }
}

void XMLUtils::insertConnections(xml_doc* doc, xml_node* node, const ConnectionList& cl){
    for(auto& conn: cl){
        auto* connNode = doc->allocate_node(rapidxml::node_element, "CONNECTION");
        connNode->append_attribute(doc->allocate_attribute("FROM", conn.from.c_str()));
        connNode->append_attribute(doc->allocate_attribute("FROM_PORT", conn.from_port.c_str()));
        connNode->append_attribute(doc->allocate_attribute("TO", conn.to.c_str()));
        connNode->append_attribute(doc->allocate_attribute("TO_PORT", conn.to_port.c_str()));
        node->append_node(connNode);
    }
}

void XMLUtils::insertConstants(xml_doc* doc, xml_node* node, const ConstantList& cl){
    for(auto& cnst: cl){
        auto* cnstNode = doc->allocate_node(rapidxml::node_element, "CONSTANT");
        cnstNode->append_attribute(doc->allocate_attribute("VALUE", cnst.value.c_str()));
        cnstNode->append_attribute(doc->allocate_attribute("TO", cnst.to.c_str()));
        cnstNode->append_attribute(doc->allocate_attribute("TO_PORT", cnst.to_port.c_str()));
        node->append_node(cnstNode);
    }
}
