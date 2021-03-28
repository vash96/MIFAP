#pragma once

#include "MifapData.hh"
#include <set>

class MifapState {

    friend ostream& operator<< (ostream&, const MifapState&);

public:
    MifapState(const MifapInput&);
    MifapState(const MifapState&);

    MifapState& operator= (const MifapState&);

    bool operator== (const MifapState&) const;
    int operator() (int cell, int pos) const { return assignment[cell][pos]; }

    void Assign(int, int, int);
    bool IsAssigned(int cell, int channel) const { return assigned[cell].count(channel) == 1; }
    const set<int>& Assigned(int cell) const { return assigned[cell]; }

    int Bch(int cell) const { return assignment[cell][0]; }
    const set<int>& Tch(int cell) const { return tch[cell]; }

    int CoCell(int cell) const { return in.CoCell(cell); }
    int CoSite() const { return in.CoSite(); }
    int AdjSep(int from, int to) const { return in.AdjSep(from, to); }

    int InRange(int, int, int, int=-1) const;


    void Reset();


protected:

    const MifapInput & in;
    vector<vector<int>> assignment;
    vector<set<int>> assigned, tch;
};