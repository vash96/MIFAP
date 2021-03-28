#pragma once

#include <map>
#include <vector>

using namespace std;


/**
 * Wraps a mapping from parametric objects to integer ids with knowledge of inverse mapping
 **/
template<typename From>
class Mapping {
public:
    Mapping() { }

    const int Add(const From & from) {
        if(direct.count(from) == 0) {
            direct.emplace(from, inverse.size());
            inverse.emplace_back(from);
        }

        return direct.at(from);
    }

    const int Id(const From & from) const {
        return direct.at(from);
    }

    const From& Inverse(const int id) const {
        return inverse[id];
    }

protected:
    map<From, int> direct;
    vector<From> inverse;

};