/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef COMPBUILDER_H
#define COMPBUILDER_H
#include <string>
#include <set>

#include "blockspec.h"

///
/// \brief Builder for composite block files (*.comp)
///
class CompBuilder{
friend class BlockBuilder;
public:
    CompBuilder(CompBuilder const&) = delete;
    void operator=(CompBuilder const&) = delete;

    //May throw
    bool buildComp(const std::string& headerPath, const CompSpec& comp,
                   std::set<std::string>* toBeBuilt = nullptr);

protected:
    CompBuilder(const std::string& libPath);

private:
    const std::string& m_libPath;
};

#endif // COMPBUILDER_H
