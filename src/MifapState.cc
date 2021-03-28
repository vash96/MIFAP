#include "MifapState.hh"


ostream& operator<< (ostream & out, const MifapState & mv) {
    for(auto & cell : mv.assignment) {
        for(auto & v : cell)
            out << v << ' ';
        out << '\n';
    }

    return out;
}





MifapState::MifapState (const MifapInput & in) 
    : in(in), assignment(in.Cells()), assigned(in.Cells()), tch(in.Cells()) {
    for(int cell=0; cell<in.Cells(); ++cell) {
        assignment[cell].resize(in.Demand(cell), -1);
    }
}

MifapState::MifapState (const MifapState & other)
    : in(other.in), assignment(other.assignment), assigned(other.assigned), tch(other.tch) {

}

MifapState& MifapState::operator= (const MifapState& other) {
    assignment = other.assignment;
    assigned = other.assigned;
    tch = other.tch;

    return *this;
}



bool MifapState:: operator== (const MifapState & other) const {
    for(int cell=0; cell<in.Cells(); ++cell) {
        if(Bch(cell) != other.Bch(cell) or assigned != other.assigned) {
            return false;
        }
    }
    return true;
}

void MifapState:: Assign(int cell, int channel, int frequency) {
    int & old = assignment[cell][channel];
    assigned[cell].erase(old);
    if(channel > 0) {
        tch[cell].erase(old);
    }
    old = frequency;
    assigned[cell].insert(frequency);
    if(channel > 0) {
        tch[cell].insert(frequency);
    }
}


int MifapState:: InRange(int l, int r, int cell, int notConsidered) const {
    return 
        l > r ? 
            0 :
            count_if(
                Assigned(cell).lower_bound(l),
                Assigned(cell).upper_bound(r),
                [notConsidered](int x) { return x != notConsidered; }
            );
}



void MifapState:: Reset() {
    for(int cell=0; cell<in.Cells(); ++cell) {
        assignment[cell] = vector<int>(in.Demand(cell), -1);
        assigned[cell].clear();
        tch[cell].clear();
    }
}