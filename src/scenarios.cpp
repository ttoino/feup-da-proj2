#include <algorithm>
#include <functional>
#include <fstream>
#include <limits>
#include <list>
#include <set>
#include <sstream>
#include <iostream>

#include "../includes/constants.hpp"
#include "../includes/scenarios.hpp"
#include "../includes/subscenarios.hpp"

ScenarioResult::ScenarioResult(int flow, int maxCapacity, std::vector<int> path)
    : flow(flow), maxCapacity(maxCapacity), path(path) {}

std::string ScenarioResult::toCSV() const {
    std::stringstream out{};

    out << runtime;

    return out.str();
}

const ScenarioResult scenario1(Dataset &dataset,
                               Scenario1Strategy strat) {


    ScenarioResult result = {-1, -1, std::vector<int>()};

    switch(strat) {
        case Scenario1Strategy::FIRST:
            result = scenario1_1(dataset); 
            break;

        case Scenario1Strategy::SECOND:
            //TODO
            break;

        default:
            break;
    }

}

const ScenarioResult scenario2(Dataset &dataset,
                               Scenario2Strategy strat) {
    

    ScenarioResult result = {-1, -1, std::vector<int>()};
    
    switch(strat) {
        case Scenario2Strategy::FIRST:
            //TODO
            break;
        case Scenario2Strategy::SECOND:
            //TODO
            break;
        case Scenario2Strategy::THIRD:
            result = scenario2_3(dataset);
            break;
        case Scenario2Strategy::FOURTH:
            //TODO
            break;
        case Scenario2Strategy::FIFTH:
            //TODO
            break;

        default:
            break;
    }

    return result;
}

void runAllScenarios() {
    std::ofstream out{DATASETS_PATH + OUTPUT_FILE};
    out << OUTPUT_HEADER;

    for (const auto &name : Dataset::getAvailableDatasets()) {
        Dataset dataset = Dataset::load(name);

        FOR_ENUM(Scenario1Strategy, strat) {
            auto result = scenario1(dataset, strat);

            out << name << ',' << 1 << ',' << (int)strat << ','
                << result.toCSV() << '\n';
        }

        FOR_ENUM(Scenario2Strategy, strat) {
            auto result = scenario2(dataset, strat);

            out << name << ',' << 2 << ',' << (int)strat << ','
                << result.toCSV() << '\n';
        }
    }
}
