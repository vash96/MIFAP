#include "MifapNexplorer.hh"

void ExchangeNexplorer:: MakeMove(MifapState & state, const MifapExchange & mv) const {
    state.Assign(mv.Cell(), mv.Channel(), mv.New());
}

void ExchangeNexplorer:: RandomMove(const MifapState & state, MifapExchange & mv) const {
    int cell = Random::Uniform<int>(0, in.Cells()-1);
    int channel = Random::Uniform<int>(0, in.Demand(cell)-1);
    
    mv.Cell() = cell;
    mv.Channel() = channel;
    mv.Old() = state(cell, channel);

    const auto & avl = in.AvailableLst(cell);
    do {
        mv.New() = avl[ Random::Uniform<unsigned>(0, avl.size()-1) ];
    }while(state.IsAssigned(cell, mv.New()));
}

// @require at least 2 channels in spectrum
void ExchangeNexplorer:: FirstMove(const MifapState & state, MifapExchange & mv) const {
    mv.Cell() = 0;
    mv.Channel() = 0;
    mv.Old() = state(mv.Cell(), mv.Channel());

    mv.New() = *find_if( // Find first free not already assigned frequency
        in.AvailableLst(mv.Cell()).begin(),
        in.AvailableLst(mv.Cell()).end(),
        [&state, &mv](int x) {
            return not state.IsAssigned(mv.Cell(), x);
        }
    );
}

bool ExchangeNexplorer:: NextMove(const MifapState & state, MifapExchange & mv) const {
    auto avl = in.AvailableLst(mv.Cell());
    unsigned posF = lower_bound(avl.begin(), avl.end(), mv.New()) - avl.begin();
    do {
        ++posF;
        if(posF == avl.size()) {
            posF = 0;
            ++mv.Channel();
            if(mv.Channel() == in.Demand(mv.Cell())) {
                mv.Channel() = 0;
                ++mv.Cell();
                if(mv.Cell() == in.Cells()) {
                    return false;
                }
                avl = in.AvailableLst(mv.Cell());
            }
        }

        mv.Old() = state(mv.Cell(), mv.Channel());
        mv.New() = avl[posF];

    } while(not FeasibleMove(state, mv));

    return true;
}


// A move is a triplet <cell, pos, newval> and is feasible iff
bool ExchangeNexplorer:: FeasibleMove(const MifapState & state, const MifapExchange & mv) const {
    return not state.IsAssigned(mv.Cell(), mv.New())
        and state.IsAssigned(mv.Cell(), mv.Old());
}