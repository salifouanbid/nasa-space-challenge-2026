#include "AstraCore/MissionCore.hpp"

#include <algorithm>
#include <utility>

namespace astra {

MissionCore::MissionCore(MissionConfig config, std::vector<SciencePoint> points)
    : config_(std::move(config)), resources_(config_.initial), points_(std::move(points)) {}

bool MissionCore::start() {
    if (outcome_ != MissionOutcome::InProgress || resources_.mass > config_.limits.mass) {
        outcome_ = MissionOutcome::Interrupted;
        reason_ = "Invalid initial configuration";
        return false;
    }
    return canContinue();
}

bool MissionCore::spend(Resource resource, double amount) {
    if (amount < 0.0 || outcome_ != MissionOutcome::InProgress) return false;
    double* value = nullptr;
    switch (resource) {
        case Resource::Energy: value = &resources_.energy; break;
        case Resource::Oxygen: value = &resources_.oxygen; break;
        case Resource::Time: value = &resources_.timeRemaining; break;
        case Resource::Data: value = &resources_.data; break;
        case Resource::Communications: value = &resources_.communications; break;
        case Resource::Budget: value = &resources_.budget; break;
        case Resource::Mass: value = &resources_.mass; break;
    }
    if (value == nullptr || *value < amount) return false;
    *value -= amount;
    interruptIfNecessary();
    return outcome_ == MissionOutcome::InProgress;
}

bool MissionCore::observe(const std::string& pointId, bool hasSpectrometer, bool hasCamera) {
    if (outcome_ != MissionOutcome::InProgress) return false;
    SciencePoint* point = findPoint(pointId);
    if (point == nullptr || point->visited) return false;
    if ((point->requiresSpectrometer && !hasSpectrometer) ||
        (point->requiresCamera && !hasCamera)) return false;
    if (resources_.data + point->dataProduced > config_.limits.data) return false;
    if (resources_.mass + point->sampleMass > config_.limits.mass) return false;
    if (!spend(Resource::Energy, point->energyCost)) return false;
    if (!spend(Resource::Time, point->timeCost)) return false;
    point->visited = true;
    resources_.data += point->dataProduced;
    scienceScore_ += point->scienceValue;
    if (point->sampleMass > 0.0) {
        resources_.mass += point->sampleMass;
        ++samples_;
    }
    return true;
}

bool MissionCore::finish() {
    if (outcome_ != MissionOutcome::InProgress) return false;
    const std::size_t visited = static_cast<std::size_t>(std::count_if(
        points_.begin(), points_.end(), [](const SciencePoint& p) { return p.visited; }));
    if (visited >= config_.requiredPoints && scienceScore_ >= config_.successScienceThreshold) {
        outcome_ = MissionOutcome::Complete;
        reason_ = "Primary objectives completed";
    } else if (visited > 0 || scienceScore_ > 0.0) {
        outcome_ = MissionOutcome::Partial;
        reason_ = "Some objectives completed";
    } else {
        outcome_ = MissionOutcome::ScientificFailure;
        reason_ = "Insufficient scientific data";
    }
    return true;
}

MissionReport MissionCore::report() const {
    MissionReport result;
    result.outcome = outcome_;
    result.scienceScore = scienceScore_;
    result.collectedSamples = samples_;
    result.reason = reason_;
    result.visitedPoints = static_cast<std::size_t>(std::count_if(
        points_.begin(), points_.end(), [](const SciencePoint& p) { return p.visited; }));
    return result;
}

SciencePoint* MissionCore::findPoint(const std::string& pointId) {
    for (auto& point : points_) if (point.id == pointId) return &point;
    return nullptr;
}

bool MissionCore::canContinue() const {
    return resources_.energy > 0.0 && resources_.oxygen > 0.0 && resources_.timeRemaining > 0.0;
}

void MissionCore::interruptIfNecessary() {
    if (resources_.oxygen <= 0.0) {
        outcome_ = MissionOutcome::Interrupted;
        reason_ = "Oxygen reserve exhausted";
    } else if (resources_.timeRemaining <= 0.0) {
        outcome_ = MissionOutcome::Interrupted;
        reason_ = "Mission time exhausted";
    }
}

} // namespace astra
