#pragma once

#include <easylocal.hh>
#include "MifapCostComponent.hh"
#include "MifapMove.hh"

using namespace EasyLocal::Core;

class SeparationDCC : public DeltaCostComponent<MifapInput, MifapState, MifapExchange, double> {
public:
    SeparationDCC(const MifapInput & in, SeparationCC & cc)
        : DeltaCostComponent<MifapInput, MifapState, MifapExchange, double>(in, cc, "SeparationDeltaCostComponent") { }

    double ComputeDeltaCost(const MifapState&, const MifapExchange&) const;

protected:
    int DeltaCoCell(const MifapState&, const MifapExchange&) const;
    int DeltaCoSite(const MifapState&, const MifapExchange&) const;
    int DeltaAdj(const MifapState&, const MifapExchange&) const;
    int DeltaHandover(const MifapState&, const MifapExchange&) const;
    
};


class InterferenceDCC : public DeltaCostComponent<MifapInput, MifapState, MifapExchange, double> {
public:
    InterferenceDCC(const MifapInput & in, InterferenceCC & cc)
        : DeltaCostComponent<MifapInput, MifapState, MifapExchange, double>(in, cc, "InterferenceDeltaCostComponent") { }

    double ComputeDeltaCost(const MifapState&, const MifapExchange&) const;

};