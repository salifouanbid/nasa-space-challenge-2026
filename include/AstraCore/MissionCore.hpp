#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace astra {

enum class MissionOutcome { InProgress, Complete, Partial, Interrupted, ScientificFailure };
enum class Resource { Energy, Oxygen, Time, Data, Communications, Budget, Mass };

struct ResourceState {
    double energy = 0.0;
    double oxygen = 0.0;
    double timeRemaining = 0.0;
    double data = 0.0;
    double communications = 0.0;
    double budget = 0.0;
    double mass = 0.0;
};

struct ResourceLimits {
    double energy = 0.0;
    double oxygen = 0.0;
    double time = 0.0;
    double data = 0.0;
    double communications = 0.0;
    double budget = 0.0;
    double mass = 0.0;
};

struct SciencePoint {
    std::string id;
    std::string category;
    std::string nameKey;
    std::string descriptionKey;
    double scienceValue = 0.0;
    double dataProduced = 0.0;
    double energyCost = 0.0;
    double timeCost = 0.0;
    double sampleMass = 0.0;
    bool requiresSpectrometer = false;
    bool requiresCamera = false;
    bool visited = false;
};

struct MissionConfig {
    std::string scenarioId;
    ResourceState initial;
    ResourceLimits limits;
    double successScienceThreshold = 0.0;
    std::size_t requiredPoints = 0;
};

struct MissionReport {
    MissionOutcome outcome = MissionOutcome::InProgress;
    double scienceScore = 0.0;
    std::size_t visitedPoints = 0;
    std::size_t collectedSamples = 0;
    std::string reason;
};

class MissionCore {
public:
    explicit MissionCore(MissionConfig config, std::vector<SciencePoint> points);

    bool start();
    bool observe(const std::string& pointId, bool hasSpectrometer, bool hasCamera);
    bool spend(Resource resource, double amount);
    bool finish();

    const ResourceState& resources() const { return resources_; }
    const std::vector<SciencePoint>& points() const { return points_; }
    MissionReport report() const;
    MissionOutcome outcome() const { return outcome_; }

private:
    SciencePoint* findPoint(const std::string& pointId);
    bool canContinue() const;
    void interruptIfNecessary();

    MissionConfig config_;
    ResourceState resources_;
    std::vector<SciencePoint> points_;
    MissionOutcome outcome_ = MissionOutcome::InProgress;
    double scienceScore_ = 0.0;
    std::size_t samples_ = 0;
    std::string reason_;
};

} // namespace astra
