#include "MifapStateManager.hh"

void MifapStateManager:: RandomState(MifapState & state) {
    state.Reset();

    vector<int> allFreqs(in.Freqs());
    iota(allFreqs.begin(), allFreqs.end(), in.Min());

    for(int cell=0; cell<in.Cells(); ++cell) {
        assert(in.Demand(cell) <= static_cast<int>(in.AvailableLst(cell).size()));

        shuffle(allFreqs.begin(), allFreqs.end(), Random::GetGenerator());
        for(int i=0, channel=0; channel<in.Demand(cell); ++i) {
            assert(i < static_cast<int>(allFreqs.size()));

            if(in.IsAvailable(cell, allFreqs[i])) {
                state.Assign(cell, channel, allFreqs[i]);
                ++channel;
            }
        }
    }
}

bool MifapStateManager:: CheckConsistency(const MifapState & state) const {
    for(int cell=0; cell<in.Cells(); ++cell) {
        if(state.Assigned(cell).count(-1) > 0) {
            return false;
        }

        if(static_cast<int>(state.Assigned(cell).size()) != in.Demand(cell)) {
            return false;
        }

        if(state.Assigned(cell).size() != 1+state.Tch(cell).size()) {
            return false;
        }

        for(auto f : state.Tch(cell)) {
            if(not state.IsAssigned(cell, f)) {
                return false;
            }
        }
    }
    return true;
}