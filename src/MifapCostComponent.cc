#include "MifapCostComponent.hh"




double SeparationCC:: ComputeCost(const MifapState & state) const {
    return static_cast<double>(
        CoCell(state) + CoSite(state)
        + Adj(state) + Handover(state));
}

void SeparationCC:: PrintViolations(const MifapState & state, ostream & out) const {
    PrintCoCell(state, out);
    PrintCoSite(state, out);
    PrintAdj(state, out);
    PrintHandover(state, out);
}


int SeparationCC:: CoCell(const MifapState & state) const {
    int cnt=0;
    for(int cell=0; cell<in.Cells(); ++cell) {
        for(int i=0; i+1<in.Demand(cell); ++i) {
            for(int j=i+1; j<in.Demand(cell); ++j) {
                cnt += in.CoCellViolation(cell, state(cell, i), state(cell, j));
            }
        }
    }

    return cnt;
}

int SeparationCC:: CoSite(const MifapState & state) const {
    int cnt=0;
    for(int s=0; s<in.Sites(); ++s) {
        for(const auto cell1 : in.Site(s)) {
            for(const auto cell2 : in.Site(s)) {
                if(cell1 == cell2) continue;

                for(const auto f : state.Assigned(cell1)) {
                    for(const auto g : state.Assigned(cell2)) {
                        cnt += in.CoSiteViolation(f, g);
                    }
                }
            }
        }
    }

    return cnt;
}

int SeparationCC:: Adj(const MifapState & state) const {
    int cnt=0;
    for(int r=0; r<in.Relations(); ++r) {
        const auto & rel = in.RelationOf(r);
        for(const auto f : state.Assigned(rel.from)) {
            for(const auto g : state.Assigned(rel.to)) {
                cnt += in.AdjViolation(rel.from, rel.to, f, g);
            }
        }
    }

    return cnt;
}

int SeparationCC:: Handover(const MifapState & state) const {
    int cnt=0;
    for(int r=0; r<in.Relations(); ++r) {
        const auto & rel = in.RelationOf(r);
        if(rel.handover) {
            int from = rel.from;
            int to   = rel.to;

            cnt += in.HandoverViolation(state.Bch(from), state.Bch(to), ChType::BCH_BCH);
            for(auto t : state.Tch(to)) {
                cnt += in.HandoverViolation(state.Bch(from), t, ChType::BCH_TCH);
            }
            for(auto t : state.Tch(from)) {
                cnt += in.HandoverViolation(t, state.Bch(to), ChType::TCH_BCH);
                for(auto tt : state.Tch(to)) {
                    cnt += in.HandoverViolation(t, tt, ChType::TCH_TCH);
                }
            }
        }
    }

    return cnt;
}





void SeparationCC:: PrintCoCell(const MifapState & state, ostream & out) const {
    for(int cell=0; cell<in.Cells(); ++cell) {
        for(int i=0; i+1<in.Demand(cell); ++i) {
            for(int j=i+1; j<in.Demand(cell); ++j) {
                if(in.CoCellViolation(cell, state(cell, i), state(cell, j))) {
                    out << "CoCell violation: [cell " << cell << "] [frequencies ("
                        << state(cell, i) << ", " << state(cell, j) << "]\n";
                }
            }
        }
    }
}

void SeparationCC:: PrintCoSite(const MifapState & state, ostream & out) const {
    for(int s=0; s<in.Sites(); ++s) {
        for(const auto cell1 : in.Site(s)) {
            for(const auto cell2 : in.Site(s)) {
                if(cell1 == cell2) continue;

                for(const auto f : state.Assigned(cell1)) {
                    for(const auto g : state.Assigned(cell2)) {
                        if(in.CoSiteViolation(f, g)) {
                            out << "CoSite violation: [cells " << cell1 << ", " << cell2
                                << "] [frequencies " << f << ", " << g << "]\n";
                        }
                    }
                }
            }
        }
    }
}

void SeparationCC:: PrintAdj(const MifapState & state, ostream & out) const {
    for(int r=0; r<in.Relations(); ++r) {
        const auto & rel = in.RelationOf(r);
        for(const auto f : state.Assigned(rel.from)) {
            for(const auto g : state.Assigned(rel.to)) {
                if(in.AdjViolation(rel.from, rel.to, f, g)) {
                    out << "Adj violation: [cells " << rel.from << ", " << rel.to
                                << "] [frequencies " << f << ", " << g << "]\n";
                }
            }
        }
    }
}

void SeparationCC:: PrintHandover(const MifapState & state, ostream & out) const {
    for(int r=0; r<in.Relations(); ++r) {
        const auto & rel = in.RelationOf(r);
        if(rel.handover) {
            int from = rel.from;
            int to   = rel.to;

            if(in.HandoverViolation(state.Bch(from), state.Bch(to), ChType::BCH_BCH)) {
                out << "Hadover violation: [cells " << from << ", " << to
                    << "] [frequencies " << state.Bch(from) << ", " << state.Bch(to) << "] [type "
                    << "bcch -> bcch]\n";
            }
            for(auto t : state.Tch(to)) {
                if(in.HandoverViolation(state.Bch(from), t, ChType::BCH_TCH)) {
                    out << "Hadover violation: [cells " << from << ", " << to
                        << "] [frequencies " << state.Bch(from) << ", " << t << "] [type "
                        << "bcch -> tch]\n";
                }
            }
            for(auto t : state.Tch(from)) {
                if(in.HandoverViolation(t, state.Bch(to), ChType::TCH_BCH)) {
                    out << "Hadover violation: [cells " << from << ", " << to
                        << "] [frequencies " << t << ", " << state.Bch(to) << "] [type "
                        << "tch -> bcch]\n";
                }
                for(auto tt : state.Tch(to)) {
                    if(in.HandoverViolation(t, tt, ChType::TCH_TCH)) {
                        out << "Hadover violation: [cells " << from << ", " << to
                            << "] [frequencies " << t << ", " << tt << "] [type "
                            << "tch -> tch]\n";
                    }
                }
            }
        }
    }
}




////////////////////////////////////////////////////////////////////////////////////

double InterferenceCC:: ComputeCost(const MifapState & state) const {
    double cnt=0;
    for(int r=0; r<in.Relations(); ++r) {
        const auto & rel = in.RelationOf(r);

        for(const auto f : state.Assigned(rel.from)) {
            for(const auto g : state.Assigned(rel.to)) {
                cnt += in.AdjInterference(rel, f, g);
            }
        }
    }

    return cnt;
}

void InterferenceCC:: PrintViolations(const MifapState & state, ostream & out) const {
    cerr << "Interferences...\n";
}