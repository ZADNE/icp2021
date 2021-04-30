/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "specstash.h"

#include <filesystem>
#include <ios>

#include "xmlutils.h"

SpecStash::SpecStash(std::string libPath):
    p_libPath(libPath)
{

}

const BlockSpec& SpecStash::operator[](const std::string& filePath){
    auto it = p_stash.find(filePath);
    if (it != p_stash.end()){
        //Spec found
        return it->second;
    }
    auto ext = std::filesystem::path(filePath).extension().string();
    if (ext == ".atom"){
        AtomSpec atom;
        if (!XMLUtils::readAtom(p_libPath + '/' + filePath, atom)) throw std::ios_base::failure{filePath};
        it = p_stash.insert(std::make_pair(filePath, atom)).first;
        return it->second;
    } else if (ext == ".comp"){
        CompSpec comp;
        if (!XMLUtils::readComp(p_libPath + '/' + filePath, comp)) throw std::ios_base::failure{filePath};
        it = p_stash.insert(std::make_pair(filePath, comp)).first;
        return it->second;
    } else {
        throw std::ios_base::failure{filePath};
    }
}
