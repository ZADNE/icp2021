/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef SPECSTASH_H
#define SPECSTASH_H
#include <map>
#include <string>

#include "blockspec.h"

class SpecStash{
public:
    SpecStash(std::string libPath);

    const BlockSpec& operator[](const std::string& filePath);

private:
    std::map<std::string, BlockSpec> p_stash;
    std::string p_libPath;
};

#endif // SPECSTASH_H
