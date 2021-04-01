#pragma once

#include <easylocal.hh>
#include "MifapState.hh"
#include "MifapData.hh"

using namespace EasyLocal::Core;




// --------
class SeparationCC : public CostComponent<MifapInput, MifapState, double> {
public:
    SeparationCC(const MifapInput & in)
        : CostComponent<MifapInput, MifapState, double>(in, 1, true, "SeparationCostComponent") { }

    double ComputeCost(const MifapState&) const;
    void PrintViolations(const MifapState&, ostream& =cerr) const;

protected:
    int CoCell(const MifapState&) const;
    int CoSite(const MifapState&) const;
    int Adj(const MifapState&) const;
    int Handover(const MifapState&) const;

    void PrintCoCell(const MifapState&, ostream& =cerr) const;
    void PrintCoSite(const MifapState&, ostream& =cerr) const;
    void PrintAdj(const MifapState&, ostream& =cerr) const;
    void PrintHandover(const MifapState&, ostream& =cerr) const;

};

////////////////////////////////////////////////////////////////////////////

class InterferenceCC : public CostComponent<MifapInput, MifapState, double> {
public:
    InterferenceCC(const MifapInput & in)
        : CostComponent<MifapInput, MifapState, double>(in, 1, false, "InterferenceCostComponent") { }

    double ComputeCost(const MifapState&) const;
    void PrintViolations(const MifapState&, ostream& =cerr) const;
};