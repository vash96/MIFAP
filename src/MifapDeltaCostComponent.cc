#include "MifapDeltaCostComponent.hh"



double SeparationDCC:: ComputeDeltaCost(const MifapState & state, const MifapExchange & mv) const {
    return static_cast<double>(
        DeltaCoCell(state, mv) + DeltaCoSite(state, mv)
        + DeltaAdj(state, mv) + DeltaHandover(state, mv));
}

int SeparationDCC:: DeltaCoCell(const MifapState & state, const MifapExchange & mv) const {
    int delta = 0;
    // for(const auto f : state.Assigned(mv.Cell())) {
    //     if(f != mv.Old()) {
    //         delta += in.CoCellViolation(mv.Cell(), f, mv.New())
    //                 - in.CoCellViolation(mv.Cell(), f, mv.Old());
    //     }
    // }
    int l=mv.New() - state.CoCell(mv.Cell()) + 1;
    int r=mv.New() + state.CoCell(mv.Cell()) - 1;

    delta += state.InRange(l, r, mv.Cell(), mv.Old());

    l=mv.Old() - state.CoCell(mv.Cell()) + 1;
    r=mv.Old() + state.CoCell(mv.Cell()) - 1;

    delta -= state.InRange(l, r, mv.Cell(), mv.Old());


    return delta;
}

int SeparationDCC:: DeltaCoSite(const MifapState & state, const MifapExchange & mv) const {
    int delta = 0;
    for(const auto cell : in.Site( in.SiteOf(mv.Cell()) )) {
        if(cell == mv.Cell()) continue;
        
        // for(const auto f : state.Assigned(cell)) {
        //     delta += in.CoSiteViolation(f, mv.New())
        //             - in.CoSiteViolation(f, mv.Old());
        // }
        
        int l=mv.New() - state.CoSite() + 1;
        int r=mv.New() + state.CoSite() - 1;

        delta += state.InRange(l, r, cell);
        
        l=mv.Old() - state.CoSite() + 1;
        r=mv.Old() + state.CoSite() - 1;

        delta -= state.InRange(l, r, cell);
    }

    return 2*delta;
}

int SeparationDCC:: DeltaAdj(const MifapState & state, const MifapExchange & mv) const {
    int delta=0;
    
    const auto Abstraction = [&](int from, int to, int cell) {
        int l=mv.New() - state.AdjSep(from, to) + 1;
        int r=mv.New() + state.AdjSep(from, to) - 1;

        delta += state.InRange(l, r, cell);
        
        l=mv.Old() - state.AdjSep(from, to) + 1;
        r=mv.Old() + state.AdjSep(from, to) - 1;

        delta -= state.InRange(l, r, cell);        
    };


    for(const auto cell : in.Outof(mv.Cell())) {
        // for(const auto f : state.Assigned(cell)) {
        //     delta += in.AdjViolation(mv.Cell(), cell, mv.New(), f)
        //             - in.AdjViolation(mv.Cell(), cell, mv.Old(), f);
        // }

        Abstraction(mv.Cell(), cell, cell);
    }

    for(const auto cell : in.Into(mv.Cell())) {
        // for(const auto f : state.Assigned(cell)) {
        //     delta += in.AdjViolation(cell, mv.Cell(), f, mv.New())
        //             - in.AdjViolation(cell, mv.Cell(), f, mv.Old());
        // }

        Abstraction(cell, mv.Cell(), cell); 
    }

    return delta;
}

int SeparationDCC:: DeltaHandover(const MifapState & state, const MifapExchange & mv) const {
    int delta=0;
    for(const auto to : in.Outof(mv.Cell())) {
        const auto & rel = in.RelationBetween(mv.Cell(), to);
        if(rel.handover) {
            ChType ty1, ty2;
            if(mv.Channel() == 0) { // If we are replacing current broadcast we have BCH_XXX
                ty1 = BCH_BCH;
                ty2 = BCH_TCH;
            }else { // Otherwise we are replacing some carry channel and we have TCH_XXX
                ty1 = TCH_BCH;
                ty2 = TCH_TCH;
            }
            delta += in.HandoverViolation(mv.New(), state.Bch(to), ty1)
                    - in.HandoverViolation(mv.Old(), state.Bch(to), ty1);

            for(const auto g : state.Tch(to)) {
                delta += in.HandoverViolation(mv.New(), g, ty2)
                        - in.HandoverViolation(mv.Old(), g, ty2);
            }
        }
    }

    for(const auto from : in.Into(mv.Cell())) {
        const auto & rel = in.RelationBetween(from, mv.Cell());
        if(rel.handover) {
            ChType ty1, ty2;
            if(mv.Channel() == 0) { // If we are replacing current broadcast we have XXX_BCH
                ty1 = BCH_BCH;
                ty2 = TCH_BCH;
            }else { // Otherwise we are replacing some carry channel and we have XXX_TCH
                ty1 = BCH_TCH;
                ty2 = TCH_TCH;
            }
            delta += in.HandoverViolation(state.Bch(from), mv.New(), ty1)
                    - in.HandoverViolation(state.Bch(from), mv.Old(), ty1);

            for(const auto g : state.Tch(from)) {
                delta += in.HandoverViolation(g, mv.New(), ty2)
                        - in.HandoverViolation(g, mv.Old(), ty2);
            }
        }
    }

    return delta;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

double InterferenceDCC:: ComputeDeltaCost(const MifapState & state, const MifapExchange & mv) const {
    double delta = 0;

    const auto Abstraction = [&](int from, int to, int cell) {
        int adjToNew = state.Assigned(cell).count(mv.New()-1)
                        + state.Assigned(cell).count(mv.New()+1);
        int sameAsNew = state.Assigned(cell).count(mv.New());
        
        delta += adjToNew * in.AdjInterference(from, to, 0, 1)
                + sameAsNew * in.AdjInterference(from, to, 0, 0);

        int adjToOld = state.Assigned(cell).count(mv.Old()-1)
                        + state.Assigned(cell).count(mv.Old()+1);
        int sameAsOld = state.Assigned(cell).count(mv.Old());
        
        delta -= adjToOld * in.AdjInterference(from, to, 0, 1)
                + sameAsOld * in.AdjInterference(from, to, 0, 0);
    };

    for(const auto cell : in.Outof(mv.Cell())) {
        // for(const auto g : state.Assigned(cell)) {
        //     delta += in.AdjInterference(mv.Cell(), cell, mv.New(), g)
        //             - in.AdjInterference(mv.Cell(), cell, mv.Old(), g);
        // }

        Abstraction(mv.Cell(), cell, cell);
    }

    for(const auto cell : in.Into(mv.Cell())) {
        // for(const auto f : state.Assigned(cell)) {
        //     delta += in.AdjInterference(cell, mv.Cell(), f, mv.New())
        //             - in.AdjInterference(cell, mv.Cell(), f, mv.Old());
        // }

        Abstraction(cell, mv.Cell(), cell);
    }

    return delta;
}