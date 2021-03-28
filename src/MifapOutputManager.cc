#include "MifapOutputManager.hh"

// Transform state into output
void MifapOutputManager:: OutputState(const MifapState & state, MifapOutput & out) const {
    for(int cell=0; cell<in.Cells(); ++cell) {
        for(int pos=0; pos<in.Demand(cell); ++pos) {
            out(cell, pos) = state(cell, pos);
        }
    }
}

// Transform output into state
void MifapOutputManager:: InputState(MifapState & state, const MifapOutput & out) const {
    for(int cell=0; cell<in.Cells(); ++cell) {
        for(int pos=0; pos<in.Demand(cell); ++pos) {
            state.Assign(cell, pos, out(cell, pos));
        }
    }
}