/***
 * \author Tomas Dubsky (xdubsk08)
 * */
#ifndef ATOMBUILDER_H
#define ATOMBUILDER_H
#include <string>

#include "blockspec.h"

///
/// \brief Builder for atomic block files (*.atom)
///
class AtomBuilder{
friend class BlockBuilder;
public:
    AtomBuilder(AtomBuilder const&) = delete;
    void operator=(AtomBuilder const&) = delete;

    bool buildAtom(const std::string& headerPath, const AtomSpec& atom);

protected:
    AtomBuilder(const std::string& libPath);

private:
    const std::string& m_libPath;
};

#endif // ATOMBUILDER_H
