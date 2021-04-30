/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "blockcompiler.h"

#include <fstream>

#include "xmlutils.h"

BlockCompiler::BlockCompiler(){

}

BlockCompiler& BlockCompiler::get(){
    static BlockCompiler bc;
    return bc;
}

bool BlockCompiler::openLibrary(const char* path){
    m_libPath = path;
    return initLibrary();
}

void BlockCompiler::setCppCompiler(const std::string& compiler){
    m_cppCompiler = compiler + ' ';
}

bool BlockCompiler::buildAtom(const std::string& atomPath){
    AtomSpec atom;
    if (!readAtom(atomPath, atom)) return false;
    std::string headerPath = atomPath + ".hpp";
    return atomC.buildAtom(headerPath, atom);
}

bool BlockCompiler::readAtom(const std::string& atomPath, AtomSpec& atom){
    return XMLUtils::readAtom(atomPath, atom);
}

bool BlockCompiler::writeAtom(const std::string& atomPath, const AtomSpec& atom){
    return XMLUtils::writeAtom(atomPath, atom);
}

bool BlockCompiler::buildComp(const std::string& compPath){
    CompSpec comp;
    if (!readComp(compPath, comp)) return false;
    std::string headerPath = compPath + ".hpp";
    try {
        return compC.buildComp(headerPath, comp);
    }  catch (...) {
        return false;
    }
}

bool BlockCompiler::readComp(const std::string& compPath, CompSpec& comp){
    return XMLUtils::readComp(compPath, comp);
}

bool BlockCompiler::writeComp(const std::string& compPath, const CompSpec& comp){
    return XMLUtils::writeComp(compPath, comp);
}

bool BlockCompiler::buildAppl(const std::string& applPath){
    ApplSpec appl;
    if (!readAppl(applPath, appl)) return false;
    std::string filePath = applPath + ".cpp";
    try {
        return applC.buildAppl(filePath, appl);
    }  catch (...) {
        return false;
    }
}

bool BlockCompiler::readAppl(const std::string& applPath, ApplSpec& appl){
    return XMLUtils::readAppl(applPath, appl);
}

bool BlockCompiler::writeAppl(const std::string& applPath, const ApplSpec& appl){
    return XMLUtils::writeAppl(applPath, appl);
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
    return true;
}
