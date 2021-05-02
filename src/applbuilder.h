/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef APPLBUILDER_H
#define APPLBUILDER_H
#include <string>
#include <set>

#include "blockspec.h"

///
/// \brief Builder for application files (*.appl)
///
class ApplBuilder{
friend class BlockBuilder;
public:
    ApplBuilder(ApplBuilder const&) = delete;
    void operator=(ApplBuilder const&) = delete;

    //May throw
    bool buildAppl(const std::string& filePath, const ApplSpec& appl,
                   std::set<std::string>& toBeBuilt);

protected:
    ApplBuilder(const std::string& libPath);

private:
    const std::string& m_libPath;
};

#endif // APPLBUILDER_H
