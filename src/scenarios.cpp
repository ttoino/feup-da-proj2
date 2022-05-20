#include <algorithm>
#include <fstream>
#include <limits>
#include <list>
#include <sstream>

#include "../includes/constants.hpp"
#include "../includes/scenarios.hpp"

ScenarioResult::ScenarioResult(const std::chrono::microseconds &runtime)
    : runtime(runtime) {}

std::string ScenarioResult::toCSV() const {
    std::stringstream out{};

    out << runtime.count();

    return out.str();
}

const ScenarioResult scenario1(const Dataset &dataset,
                               Scenario1Strategy strat) {
    // TODO

    return {{}};
}

const ScenarioResult scenario2(const Dataset &dataset,
                               Scenario2Strategy strat) {
    // TODO

    return {{}};
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
