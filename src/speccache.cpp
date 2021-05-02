/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#include "speccache.h"

void SpecCache::setLibPath(std::string path){
    //Save all previous cached specs
    saveAll();
    m_libPath = path;
    m_stash.clear();
}

SpecCache& SpecCache::cache(){
    static SpecCache cache;
    return cache;
}

SpecCache::SpecCache(){

}

SpecCache::~SpecCache(){
    saveAll();
}

void SpecCache::saveAll(){
    for (auto& cachedSpec: m_stash){
        if (cachedSpec.second.needsSaving){
            XMLUtils::writeAny(m_libPath + cachedSpec.first,
                               cachedSpec.second.spec);
        }
    }
}

void SpecCache::dropCache(){
    saveAll();
    m_stash.clear();
}

AnySpec SpecCache::fetchAny(const std::string& path){
    return cache().fetchAnySpec(path);
}

AnySpec SpecCache::fetchAnySpec(const std::string& path){
    //Look for it in stash
    auto it = m_stash.find(path);
    if (it != m_stash.end()){
        return it->second.spec;
    }
    //Load it from file
    AnySpec readSpec = XMLUtils::readAny(m_libPath + path);
    if (!std::holds_alternative<std::monostate>(readSpec)){
        it = m_stash.insert(std::make_pair(path, CachedSpec{readSpec, false})).first;
        return it->second.spec;
    } else {
        return m_mono;
    }
}

const AnySpec SpecCache::m_mono = AnySpec{};
