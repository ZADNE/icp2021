/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef SPECCACHE_H
#define SPECCACHE_H
#include <map>
#include <string>
#include <ios>

#include "blockspec.h"
#include "xmlutils.h"

class SpecCache: public XMLUtils{
friend class BlockCompiler;
protected:
    struct CachedSpec{
        CachedSpec(const AnySpec& spec, bool needsSaving):
            spec(spec),
            needsSaving(needsSaving)
        {}

        AnySpec spec;
        bool needsSaving;//Needs to be written on disc
    };
public:
    static SpecCache& cache();
    SpecCache(SpecCache const&) = delete;
    void operator=(SpecCache const&) = delete;

    void saveAll();

    static const AnySpec& fetchAny(const std::string& path);
    const AnySpec& fetchAnySpec(const std::string& path);

    template<class Spec>static bool fetch(const std::string& path, Spec& spec){
        return cache().fetchSpec(path, spec);
    }

    template<class Spec>bool fetchSpec(const std::string& path, Spec& spec){
        AnySpec any = fetchAny(path);
        if (std::holds_alternative<Spec>(any)){
            spec = std::get<Spec>(any);
            return true;
        }
        return false;
    }

    template<class Spec>static void cache(const std::string& path, Spec& spec){
        cache().cacheSpec(path, spec);
    }
    template<class Spec>void cacheSpec(const std::string& path, Spec& spec){
        //Overwrite it in stash
        m_stash.insert_or_assign(path, CachedSpec{spec, true});
    }

    //Ensures that change is written to disc
    template<class Spec>static void save(const std::string& path, Spec& spec){
        cache().saveSpec(path, spec);
    }
    template<class Spec>void saveSpec(const std::string& path, Spec& spec){
        //Overwrite it in stash
        m_stash.insert_or_assign(path, CachedSpec{spec, false});
        AnySpec any = spec;
        XMLUtils::writeAny(m_libPath + path, any);
    }

protected:
    void setLibPath(std::string path);
private:
    SpecCache();
    ~SpecCache();

    std::map<std::string, CachedSpec> m_stash;
    std::string m_libPath;
    static const AnySpec m_mono;
};

#endif // SPECCACHE_H
