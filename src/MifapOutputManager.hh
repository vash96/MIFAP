#pragma once

#include <easylocal.hh>
#include "MifapData.hh"
#include "MifapState.hh"

using namespace EasyLocal::Core;

class MifapOutputManager : public OutputManager<MifapInput, MifapOutput, MifapState> {
public:
    MifapOutputManager(const MifapInput & in)
        : OutputManager<MifapInput, MifapOutput, MifapState>(in, "MifapOutputManager") { }

    void OutputState(const MifapState&, MifapOutput&) const;
    void InputState(MifapState&, const MifapOutput&) const;

};
