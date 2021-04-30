/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKSPEC_H
#define BLOCKSPEC_H
#include <string>
#include <vector>
#include <variant>

struct SlotSpec{
    SlotSpec(bool templ, std::string type, std::string name):
        templ(templ), type(type), name(name) {};

    bool templ;
    std::string type;
    std::string name;
};
using SlotList = std::vector<SlotSpec>;

struct AtomSpec{
    std::string name;
    SlotList inputs;
    SlotList outputs;
    std::string body;
    std::string stateVars;
};

//Instance of a block inside a composite block
struct InstanceSpec{
    InstanceSpec(){}
    InstanceSpec(std::string name, std::string path, int x, int y):
        name(name),
        path(path),
        x(x),
        y(y)
    {}

    std::string name;
    std::string path;
    int x;
    int y;
};
using InstanceList = std::vector<InstanceSpec>;

struct ConnectionSpec{
    ConnectionSpec(){}
    ConnectionSpec(
        std::string from,
        std::string from_port,
        std::string to,
        std::string to_port):
        from(from),
        from_port(from_port),
        to(to),
        to_port(to_port)
    {}

    std::string from;
    std::string from_port;
    std::string to;
    std::string to_port;
};
using ConnectionList = std::vector<ConnectionSpec>;

struct ConstantSpec{
    std::string value;
    std::string to;
    std::string to_port;
};
using ConstantList = std::vector<ConstantSpec>;

struct CompSpec{
    std::string name;
    SlotList inputs;
    SlotList outputs;
    InstanceList instances;
    ConnectionList connections;
    ConstantList constants;
};

using BlockSpec = std::variant<AtomSpec, CompSpec>;

struct ApplSpec{
    std::string name;
    InstanceList instances;
    ConnectionList connections;
    ConstantList constants;
};

#endif // BLOCKSPEC_H
