#include "MifapData.hh"
#include <fstream>
#include <iomanip>

istream & operator>> (istream & str, MifapInput & in) {
    in.ReadFromStream(str);
    return str;
}

ostream & operator<< (ostream & str, const MifapInput & in) {
    str << in.inputJ;
    return str;
}




MifapInput:: MifapInput(string filename) {
    ifstream in(filename);
    ReadFromStream(in);
}

void MifapInput:: ReadFromStream(istream & in) {
    in >> inputJ;

    const json & general = JExists<json>(inputJ, "general_information");

    const json & spectrum = JExists<json>(general, "spectrum");
    minF = JExists<int>(spectrum, "min");
    maxF = JExists<int>(spectrum, "max");
    nFreqs = maxF - minF + 1;
    int coCell = JExists<int>(general, "default_co_cell_separation");
    coSite = JExists<int>(general, "co_site_separation");

    const json & handoverJ = JExists<json>(general, "handover_separation");
    handover[0][0] = JExists<int>(handoverJ, "bcch->bcch");
    handover[0][1] = JExists<int>(handoverJ, "bcch->tch");
    handover[1][0] = JExists<int>(handoverJ, "tch->bcch");
    handover[1][1] = JExists<int>(handoverJ, "tch->tch");

    const auto & globalV = JExists<vector<int>>(general, "globally_blocked_channels");
    const unordered_set<int> global(globalV.begin(), globalV.end());

    const auto & cellsJ = JExists<vector<json>>(inputJ, "cells");
    nCells = cellsJ.size();
    cells.reserve(nCells);
    sites.reserve(nCells);

    for(const json & cellJ : cellsJ) {
        const string cellName = JExists<string>(cellJ, "id");
        const string siteName = JExists<string>(cellJ, "site");
        const int idCell = cellMap.Add(cellName);
        const int idSite = siteMap.Add(siteName);

        const auto & b = JExists<vector<int>>(cellJ, "blocked_channels");
        const unordered_set<int> local(b.begin(), b.end());
        
        vector<int> avl;
        for(int f=minF; f<=maxF; ++f) {
            if(global.count(f) + local.count(f) == 0) {
                avl.emplace_back(f);
            }
        }

        cells.emplace_back(
            idCell,
            idSite,
            JExists<int>(cellJ, "demand"),
            JExists<int>(cellJ, "separation", coCell),
            avl
        );

        if(idSite == static_cast<int>(sites.size())) {
            sites.emplace_back();
        }

        sites[idSite].emplace_back(idCell);
    }

    
    const auto & relationsJ = JExists<vector<json>>(inputJ, "cell_relations");
    relations.reserve(nCells * nCells);
    outof.resize(nCells);
    into.resize(nCells);

    for(const json & relationJ : relationsJ) {
        const string fromName = JExists<string>(relationJ, "from");
        const string toName = JExists<string>(relationJ, "to");

        const int idFrom = cellMap.Id(fromName);
        const int idTo = cellMap.Id(toName);

        double sameInterf = 0.0;
        double adjInterf = 0.0;

        if(relationJ.contains("downlink_area_interference")) {
            sameInterf = relationJ["downlink_area_interference"].value("same_channel", 0.0);
            adjInterf = relationJ["downlink_area_interference"].value("adjacent_channel", 0.0);
        }

        relations.emplace_back(
            JExists<bool>(relationJ, "handover", false),
            JExists<int>(relationJ, "separation", 0),
            idFrom,
            idTo,
            sameInterf,
            adjInterf
        );

        assert(idFrom < nCells);
        assert(idTo < nCells);
        const int idRel = static_cast<int>(relations.size()) - 1;

        cells[idFrom].relations.emplace(idTo, idRel);
        outof[idFrom].emplace_back(idTo);
        into[idTo].emplace_back(idFrom);
    }
}




template<typename T>
const T MifapInput:: JExists(const json & J, const char* name, const T& defaultV) const {
    if(not J.contains(name)) {
        // cerr << "Missing attribute \"" << name << "\", please check json input." << endl;
    }

    return J.value(name, defaultV);
}



//////////////////////////////////////////////////////////////////////////////

// @TODO
ostream& operator<< (ostream & out, const MifapOutput & in) {
    json outJ;

    outJ["format"] = json::object({
        {"scenario", "assignment"},
        {"version", 1.0}
    });
    outJ["general_information"] = json::object({
        {"scenario_id", in.in.J()["general_information"]["scenario_id"]},
        {"name", "frent"},
        {"annotation", "Brian Riccardi, Advanced Scheduling Systems"}
    });

    for(int cell=0; cell<in.in.Cells(); ++cell) {
        json cellJ;
        cellJ["id"] = in.in.Name(cell);

        for(const auto f : in.assignment[cell]) {
            cellJ["assignments"].emplace_back(json::object({
                {"channel", f},
                {"changeable", true}
            }));
        }

        outJ["cells"].emplace_back(cellJ);
    }

    out << setw(4) << outJ << endl;

    return out;
}

// @TODO
istream& operator>> (istream & in, MifapOutput & out) {
    json outJ;
    in >> outJ;
    
    for(const json & cellJ : outJ["cells"]) {
        int cell = out.in.Id(cellJ["id"]);
        assert(cell < out.in.Cells());
        assert(out.in.Demand(cell) == static_cast<int>(cellJ["assignments"].size()));
        for(int channel=0; channel<out.in.Demand(cell); ++channel) {
            out(cell, channel) = cellJ["assignments"][channel]["channel"];
        }
    }

    return in;
}


MifapOutput:: MifapOutput(const MifapInput & in)
    : in(in), assignment(in.Cells()) {
    for(int cell=0; cell<in.Cells(); ++cell) {
        assignment[cell].resize(in.Demand(cell));
    }
}