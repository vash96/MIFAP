#pragma once

#include <utils.hh>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "map_utils.hh"
using namespace std;

using namespace std;
using json = nlohmann::json;


enum ChType {
    BCH_BCH=0, BCH_TCH=1,
    TCH_BCH=2, TCH_TCH=3
};

class MifapInput {
    struct Cell {
        int id, site, demand, separation;
        vector<int> availableFreqsLst;
        unordered_set<int> availableFreqs;
        unordered_map<int, int> relations;

        Cell(int i, int si, int d, int se, vector<int> a)
        : id(i), site(si), demand(d), separation(se),
            availableFreqsLst(move(a)), availableFreqs(availableFreqsLst.begin(), availableFreqsLst.end()) {
                assert(availableFreqsLst.size() > 0);
                assert(availableFreqsLst.size() == availableFreqs.size());
            }
    };

    struct Relation {
        bool handover;
        int separation, from, to;
        double sameChannel, adjChannel;

        Relation(bool h, int s, int f, int t, double same, double adj)
        : handover(h), separation(s), from(f), to(t), sameChannel(same), adjChannel(adj) { }
    };



    friend ostream& operator<< (ostream&, const MifapInput&);
    friend istream& operator>> (istream&, MifapInput&);

public:
    // Constructor
    MifapInput(string);

    // Getter
    const json& J() const { return inputJ; }
    int Min() const { return minF; }
    int Max() const { return maxF; }
    int Freqs() const { return nFreqs; }
    int Cells() const { return nCells; }
    int Sites() const { return sites.size(); }
    int Relations() const { return relations.size(); }
    int Demand(int cell) const { return cells[cell].demand; }
    int SiteOf(int cell) const { return cells[cell].site; }
    int CoCell(int cell) const { return cells[cell].separation; }
    int CoSite() const { return coSite; }
    int AdjSep(int from, int to) const { return RelationBetween(from, to).separation; }
    string Name(int cell) const { return cellMap.Inverse(cell); }
    int Id(const string& name) const { return cellMap.Id(name); }

    const Cell& CellOf(int i) const { return cells[i]; }
    const Relation& RelationOf(int i) const { return relations[i]; }
    const Relation& RelationBetween(int i, int j) const { return relations[CellOf(i).relations.at(j)]; }
    const vector<int>& Site(int site) const { return sites[site]; }
    const vector<int>& AvailableLst(int cell) const { return cells[cell].availableFreqsLst; }
    const unordered_set<int>& AvailableSet(int cell) const { return cells[cell].availableFreqs; }
    const vector<int>& Outof(int cell) const { return outof[cell]; }
    const vector<int>& Into(int cell) const { return into[cell]; }

    // Info
    bool IsAvailable(int cell, int frequency) const {
        return cells[cell].availableFreqs.count(frequency) > 0;
    }
    bool CoCellViolation(int cell, int f, int g) const {
        return abs(f - g) < cells[cell].separation;
    }
    bool CoSiteViolation(int f, int g) const {
        return abs(f - g) < coSite;
    }
    bool AdjViolation(int cell1, int cell2, int f, int g) const {
        assert(cells[cell1].relations.count(cell2));
        return abs(f - g) < RelationBetween(cell1, cell2).separation;
    }
    bool HandoverViolation(int f, int g, ChType t) const {
        return abs(f - g) < handover[(2 & t) >> 1][1 & t];
    }
    double AdjInterference(const Relation & rel, int f, int g) const {
        if(f == g) return rel.sameChannel;
        if(abs(f - g) == 1) return rel.adjChannel;
        return 0.0;
    }
    double AdjInterference(int cell1, int cell2, int f, int g) const {
        return AdjInterference(RelationBetween(cell1, cell2), f, g);
    }

protected: // Hidden methods
    void ReadFromStream(istream&);

    template<typename T>
    const T JExists(const json&, const char*, const T& =T()) const;

protected: // Attributes and redundant data
    json inputJ;

    int minF, maxF, nFreqs, nCells, coSite;
    int handover[2][2] = {{0,0},{0,0}};

    vector<Cell> cells;                 // cells data aggregated: their index will be considered as id
    vector<Relation> relations;         // relations data aggregated: their index will be considered as id
    vector<vector<int>> sites;          // list of cells in given site
    vector<vector<int>> outof, into;    // outgoing/ingoing relation arcs for given cell

    
    Mapping<string> cellMap, siteMap;
};


class MifapOutput {
    friend ostream& operator<< (ostream&, const MifapOutput&);
    friend istream& operator>> (istream&, MifapOutput&);

public:
    MifapOutput(const MifapInput&);

    int operator() (int cell, int pos) const { return assignment[cell][pos]; }
    int& operator() (int cell, int pos) { return assignment[cell][pos]; }
    

protected:
    const MifapInput & in;
    vector<vector<int>> assignment;

};