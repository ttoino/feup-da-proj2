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

ScenarioResult::ScenarioResult(int flow, int maxCapacity, int groupSize,
    std::vector<std::list<int>> paths, const std::chrono::microseconds &runtime, int minDuration)
    : flow(flow), maxCapacity(maxCapacity), groupSize(groupSize), paths(paths), runtime(runtime), minDuration(minDuration)  {}

std::string ScenarioResult::toCSV() const {
    std::stringstream out{};
    std::list<int> path = paths.at(0);
    auto front = path.begin();

    out << "runtime" << ',' << this->flow << ',' << this->maxCapacity;

    if (!path.empty()) {
        std::stringstream path_ss;
        path_ss << "[";
        for (int i = 0; i < path.size() - 1; ++i) {
            std::advance(front, i);
            path_ss << *front << " -> ";
        }
            

        path_ss << path.back() << "]";

        out << ',' << path_ss.str();
    } else out << ",[]";

    out << ',' << minDuration;

    return out.str();
}

const ScenarioResult scenario1(Dataset &dataset,
                               Scenario1Strategy strat) {


    ScenarioResult result = {-1, -1, -1, std::vector<std::list<int>>(), static_cast<std::chrono::microseconds>(0), -1};

    switch(strat) {
        case Scenario1Strategy::FIRST:
            result = scenario1_1(dataset); 
            break;

        case Scenario1Strategy::SECOND:
            result = scenario1_2(dataset);
            break;

        default:
            break;
    }

    return result;

}

const ScenarioResult scenario2(Dataset &dataset,
                               Scenario2Strategy strat) {
    

    ScenarioResult result = {-1, -1, -1, std::vector<std::list<int>>(), static_cast<std::chrono::microseconds>(0), -1};
    
    switch(strat) {
        case Scenario2Strategy::FIRST:
            result = scenario2_1(dataset);
            break;
        case Scenario2Strategy::SECOND:
            //TODO
            break;
        case Scenario2Strategy::THIRD:
            result = scenario2_3(dataset);
            break;
        case Scenario2Strategy::FOURTH:
            //TODO
            result = scenario2_4(dataset);
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
