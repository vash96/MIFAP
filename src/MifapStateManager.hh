#pragma once

#include "MifapState.hh"
#include <easylocal.hh>

using namespace EasyLocal::Core;

class MifapStateManager : public StateManager<MifapInput, MifapState, DefaultCostStructure<double>> {
public:
    MifapStateManager(const MifapInput & in) 
        : StateManager<MifapInput, MifapState, DefaultCostStructure<double>>(in, "MifapStateManager") { }

    void RandomState(MifapState&);
    bool CheckConsistency(const MifapState&) const;

};