#include "MifapMove.hh"

ostream& operator<< (ostream & out, const MifapExchange & mv) {
    out << '<' << mv.Cell() << ',' << mv.Channel() << ',' << mv.New() << ',' << mv.Old() << '>';
    
    return out;
}



istream& operator>> (istream & in, MifapExchange & mv) {
    char dump;
    in >> dump;
    for(int i=0; i<(int)mv.mv.size(); ++i) {
        in >> mv.mv[i] >> dump;
    }

    return in;
}

bool MifapExchange:: operator== (const MifapExchange & other) const {
    return Cell() == other.Cell()
        and Channel() == other.Channel()
        and New() == other.New();
}


