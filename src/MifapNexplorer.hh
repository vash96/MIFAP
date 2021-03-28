#pragma once

#include <easylocal.hh>
#include "MifapData.hh"
#include "MifapState.hh"
#include "MifapMove.hh"
#include "MifapStateManager.hh"

using namespace EasyLocal::Core;


class ExchangeNexplorer : public NeighborhoodExplorer<MifapInput, MifapState, MifapExchange, DefaultCostStructure<double>> {
public:
    ExchangeNexplorer(const MifapInput & in, MifapStateManager & sm)
        : NeighborhoodExplorer<MifapInput, MifapState, MifapExchange, DefaultCostStructure<double>>(in, sm, "ExchangeNeighborhoodExplorer") { }

    void MakeMove(MifapState&, const MifapExchange&) const;
    void RandomMove(const MifapState&, MifapExchange&) const;
    void FirstMove(const MifapState&, MifapExchange&) const;
    bool NextMove(const MifapState&, MifapExchange&) const;
    bool FeasibleMove(const MifapState&, const MifapExchange&) const;
};