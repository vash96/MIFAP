#include "MifapData.hh"
#include "MifapStateManager.hh"
#include "MifapOutputManager.hh"
#include "MifapNexplorer.hh"
#include "MifapCostComponent.hh"
#include "MifapDeltaCostComponent.hh"
#include <easylocal.hh>

#include <fstream>

using namespace EasyLocal::Debug;
using namespace EasyLocal::Core;
using json = nlohmann::json;

int main(int argc, const char** argv)
{
    ParameterBox main_parameters("main", "Main program options");

    Parameter<string> instance("instance", "Input instance", main_parameters);
    Parameter<string> method("method", "Solution method", main_parameters);

    CommandLineParameters::Parse(argc, argv, false, true);

    if(not instance.IsSet()) {
        cerr << "Missing instance: --main::instance <filename>" << endl;
        return 1;
    }


    // data
    MifapInput in(instance);

    // helpers
    MifapStateManager sm(in);
    ExchangeNexplorer enexp(in, sm);
    MifapOutputManager outm(in);

    // cost components
    SeparationCC scc(in);
    SeparationDCC sdelta(in, scc);

    InterferenceCC icc(in);
    InterferenceDCC idelta(in, icc);

    sm.AddCostComponent(scc);
    sm.AddCostComponent(icc);

    enexp.AddDeltaCostComponent(sdelta);
    enexp.AddDeltaCostComponent(idelta);

    // Runners
    TabuSearch<MifapInput, MifapState, MifapExchange, DefaultCostStructure<double>> ets(in, sm, enexp, "ExchangeTS",
        [](const MifapExchange & lm, const MifapExchange & mv) {
            return lm.Cell() == mv.Cell()
                and lm.Old() == mv.New();
    });
    
    SimulatedAnnealing<MifapInput, MifapState, MifapExchange, DefaultCostStructure<double>> esa(in, sm, enexp, "ExchangeSA");

    // Tester
    Tester<MifapInput, MifapOutput, MifapState, DefaultCostStructure<double>> tester(in, sm, outm);
    MoveTester<MifapInput, MifapOutput, MifapState, MifapExchange, DefaultCostStructure<double>> move_tester(in, sm, outm, enexp, "ExchangeMove", tester);
    move_tester.SetTolerance(1e-4);



    // Solver
    SimpleLocalSearch<MifapInput, MifapOutput, MifapState, DefaultCostStructure<double>> solver(in, sm, outm, "MySolver");

    if(not method.IsSet()) {
        CommandLineParameters::Parse(argc, argv);

        tester.RunMainMenu();

        return 0;

    }else if(method == string("TS")) {
        solver.SetRunner(ets);
    }else if(method == string("SA")) {
        solver.SetRunner(esa);
    }else {
        cerr << "Error: valid methods are 'TS' or 'SA'" << endl;
        return 1;
    }

    CommandLineParameters::Parse(argc, argv);

    auto solution = solver.Solve();
    cout << "Elapsed time: " << solution.running_time << endl;
    cout << "Cost: " << solution.cost << endl;
    cout << "In " << solver.GetRunner()->Iteration() << " iterations" << endl;

    return 0;
}