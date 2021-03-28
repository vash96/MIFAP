#pragma once

#include "MifapData.hh"
#include <array>

using namespace std;

class MifapExchange {

    friend ostream& operator<< (ostream&, const MifapExchange&);
    friend istream& operator>> (istream&, MifapExchange&);

public:
    MifapExchange(int cell=0, int ch=0, int newVal=0, int oldVal=0)
        : mv( {{cell, ch, newVal, oldVal}} ) { }

    bool operator== (const MifapExchange&) const;
    bool operator!= (const MifapExchange & other) const { return not (*this == other); }
    bool operator< (const MifapExchange & other) const { return mv < other.mv; }

    int Cell() const    { return mv[0]; }
    int Channel() const { return mv[1]; }
    int New() const     { return mv[2]; }
    int Old() const     { return mv[3]; }

    int& Cell()    { return mv[0]; }
    int& Channel() { return mv[1]; }
    int& New()     { return mv[2]; }
    int& Old()     { return mv[3]; }

protected:
    array<int, 4> mv;
};