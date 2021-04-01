#include <bits/stdc++.h>
using namespace std;

/**
 * Elapsed time: TTT.TTT
 * Cost: CCC.CCC (viol: VVV, obj: OOO.OOO, comps: {VVV, OOO.OOO})
 * In NNN iterations
**/

int main(int argc, const char** argv)
{
    if(argc != 2) {
        cerr << "Usage: ./sa_analyzer <path>" << endl;
        return 1;
    }
    
    double min_cost = numeric_limits<double>::max(), avg_cost = 0.0, avg_time = 0.0, avg_iter = 0.0;
    unsigned min_iter = -1, max_iter = 0, viol_of_best = 0;
    string path(argv[1]);
    for(int i=1; i<=10; ++i) {
        string filename = path + to_string(i) + ".txt";
        FILE *pf = fopen(filename.c_str(), "r");

        unsigned v, it;
        double e, c, o;

        fscanf(pf, "Elapsed time: %lf\n", &e);
        fscanf(pf, "Cost: %lf (viol: %*d, obj: %*lf, comps: {%d, %lf})\n", &c, &v, &o);
        fscanf(pf, "In %d iterations", &it);

        fclose(pf);

        if(min_cost > c) {
            min_cost = c;
            viol_of_best = v;
        }
        avg_cost += c;
        avg_time += e;
        avg_iter += static_cast<double>(it);
        min_iter = min(min_iter, it);
        max_iter = max(max_iter, it);
    }

    avg_cost /= 10.0;
    avg_time /= 10.0;
    avg_iter /= 10.0;

    cout << min_cost << "\t" << viol_of_best << "\t" << avg_cost << "\t" << avg_time << "\t" << min_iter << "\t" << max_iter << "\t" << avg_iter << endl;

    

    return 0;
}