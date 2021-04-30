#ifndef SPECSTASH_H
#define SPECSTASH_H
#include <map>
#include <string>

#include "blockspec.h"

class SpecStash{
public:
    SpecStash();

    const BlockSpec& operator[](const std::string& filePath);

private:
    std::map<std::string, BlockSpec> p_stash;
};

#endif // SPECSTASH_H
