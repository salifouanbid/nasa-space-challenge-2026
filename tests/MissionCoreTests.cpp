#include "AstraCore/MissionCore.hpp"

#include <cassert>
#include <iostream>

int main() {
    astra::MissionConfig config;
    config.scenarioId = "gamma";
    config.initial.energy = 100.0;
    config.initial.oxygen = 100.0;
    config.initial.timeRemaining = 100.0;
    config.initial.communications = 100.0;
    config.initial.budget = 1000.0;
    config.initial.mass = 10.0;
    config.limits.energy = 100.0;
    config.limits.oxygen = 100.0;
    config.limits.time = 100.0;
    config.limits.data = 50.0;
    config.limits.communications = 100.0;
    config.limits.budget = 1000.0;
    config.limits.mass = 15.0;
    config.successScienceThreshold = 10.0;
    config.requiredPoints = 1;

    astra::SciencePoint point;
    point.id = "shackleton-rim-observation";
    point.scienceValue = 10.0;
    point.dataProduced = 5.0;
    point.energyCost = 10.0;
    point.timeCost = 8.0;
    point.requiresCamera = true;

    astra::MissionCore mission(config, {point});
    assert(mission.start());
    assert(mission.observe("shackleton-rim-observation", false, true));
    assert(mission.finish());
    assert(mission.report().outcome == astra::MissionOutcome::Complete);
    assert(mission.report().visitedPoints == 1);

    std::cout << "MissionCoreTests: OK\n";
    return 0;
}
