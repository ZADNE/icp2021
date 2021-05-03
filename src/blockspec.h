/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef BLOCKSPEC_H
#define BLOCKSPEC_H
#include <string>
#include <vector>
#include <variant>

///
/// \brief POD specification of a port
///
struct PortSpec{
    PortSpec(/*bool templ, */std::string type, std::string name):
        /*templ(templ), */type(type), name(name) {};

    //bool templ;
    std::string type;
    std::string name;
};
using PortList = std::vector<PortSpec>;

///
/// \brief POD specification of an atomic block
///
struct AtomSpec{
    std::string name;
    PortList inputs;
    PortList outputs;
    std::string body;
    std::string stateVars;
};

///
/// \brief POD specification of a block inside composite block
///
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

///
/// \brief POD specification of a connection between two ports
///
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

///
/// \brief POD specification a constant value installed to port
///
struct ConstantSpec{
    ConstantSpec(){}
    ConstantSpec(
        std::string value,
        std::string to,
        std::string to_port):
        value(value),
        to(to),
        to_port(to_port)
    {}
    std::string value;
    std::string to;
    std::string to_port;
};
using ConstantList = std::vector<ConstantSpec>;

///
/// \brief POD specification of a composite block
///
struct CompSpec{
    std::string name;
    PortList inputs;
    PortList outputs;
    InstanceList instances;
    ConnectionList connections;
    ConstantList constants;
};

///
/// \brief POD specification of an application
///
struct ApplSpec{
    std::string name;
    InstanceList instances;
    ConnectionList connections;
    ConstantList constants;
    size_t maxSteps;
};

using AnySpec = std::variant<std::monostate, AtomSpec, CompSpec, ApplSpec>;

#endif // BLOCKSPEC_H
