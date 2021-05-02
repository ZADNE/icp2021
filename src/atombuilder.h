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
    AtomBuilder();
};

#endif // ATOMBUILDER_H
