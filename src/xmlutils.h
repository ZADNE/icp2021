/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef XMLUTILS_H
#define XMLUTILS_H
#include "blockspec.h"

namespace rapidxml{
    template<class Ch>class xml_document;
    template<class Ch>class xml_node;
}
using xml_doc = rapidxml::xml_document<char>;
using xml_node = rapidxml::xml_node<char>;

namespace XMLUtils{

    //Full size functions for reading specs from XML file
    bool readAtom(const std::string& atomPath, AtomSpec& atom);
    bool readComp(const std::string& compPath, CompSpec& comp);
    bool readAppl(const std::string& applPath, ApplSpec& appl);

    //Full size functions for writing specs to XML file
    bool writeAtom(const std::string& atomPath, const AtomSpec& atom);
    bool writeComp(const std::string& compPath, const CompSpec& comp);
    bool writeAppl(const std::string& applPath, const ApplSpec& appl);

    //Functions for extracting specs from XML node
    void extractPorts(xml_node* node, SlotList& sl);
    bool extractInstances(xml_node* node, InstanceList& il);
    bool extractConnections(xml_node* node, ConnectionList& cl);
    bool extractContants(xml_node* node, ConstantList& cl);

    //Functions for inserting specs into XML node
    void insertPorts(xml_doc* doc, xml_node* node, const SlotList& sl);
    void insertInstances(xml_doc* doc, xml_node* node, const InstanceList& il);
    void insertConnections(xml_doc* doc, xml_node* node, const ConnectionList& cl);
    void insertConstants(xml_doc* doc, xml_node* node, const ConstantList& cl);

}

#endif // XMLUTILS_H
