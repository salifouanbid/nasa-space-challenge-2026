#include "Mission/AstraMissionRuntimeSubsystem.h"

namespace AstraRuntimePrivate
{
    bool IsFiniteNonNegative(float Value)
    {
        return FMath::IsFinite(Value) && Value >= 0.0f;
    }
}

void UAstraMissionRuntimeSubsystem::InitializeMission001RuntimeDefaults()
{
    FAstraRuntimeResourceState Initial;
    Initial.Energy = 85.0f;
    Initial.Oxygen = 100.0f;
    Initial.TimeRemaining = 90.0f;

    FAstraRuntimeResourceState Limits;
    Limits.Energy = 85.0f;
    Limits.Oxygen = 100.0f;
    Limits.TimeRemaining = 90.0f;
    Limits.Data = 45.0f;
    Limits.Mass = 13.0f;

    FAstraRuntimeSciencePoint Rim;
    Rim.PointId = TEXT("gamma-rim-observation");
    Rim.DisplayName = FText::FromString(TEXT("Relief du pôle Sud"));
    Rim.ScienceValue = 12.0f;
    Rim.DataProduced = 8.0f;
    Rim.EnergyCost = 12.0f;
    Rim.TimeCost = 14.0f;
    Rim.bRequiresCamera = true;

    FAstraRuntimeSciencePoint ShadowedSite;
    ShadowedSite.PointId = TEXT("gamma-shadowed-site");
    ShadowedSite.DisplayName = FText::FromString(TEXT("Région ombragée — cible scientifique"));
    ShadowedSite.ScienceValue = 14.0f;
    ShadowedSite.DataProduced = 12.0f;
    ShadowedSite.EnergyCost = 18.0f;
    ShadowedSite.TimeCost = 18.0f;
    ShadowedSite.SampleMass = 0.6f;
    ShadowedSite.bRequiresSpectrometer = true;

    TArray<FAstraRuntimeSciencePoint> Points;
    Points.Add(Rim);
    Points.Add(ShadowedSite);

    ConfigureRuntimeMission(
        TEXT("gamma"),
        Initial,
        Limits,
        2,
        24.0f,
        Points);
}

void UAstraMissionRuntimeSubsystem::ConfigureRuntimeMission(
    FName MissionId,
    const FAstraRuntimeResourceState& InitialResources,
    const FAstraRuntimeResourceState& NewResourceLimits,
    int32 NewRequiredPoints,
    float NewSuccessScienceThreshold,
    const TArray<FAstraRuntimeSciencePoint>& NewSciencePoints)
{
    RuntimeMissionId = MissionId;
    this->InitialResources = InitialResources;
    Resources = InitialResources;
    ResourceLimits = NewResourceLimits;
    RequiredPoints = FMath::Max(0, NewRequiredPoints);
    SuccessScienceThreshold = FMath::Max(0.0f, NewSuccessScienceThreshold);
    SciencePoints = NewSciencePoints;
    ScienceScore = 0.0f;
    CollectedSamples = 0;
    Outcome = EAstraRuntimeMissionOutcome::NotStarted;
    OutcomeReason.Empty();

    for (FAstraRuntimeSciencePoint& Point : SciencePoints)
    {
        Point.bVisited = false;
    }
    OnRuntimeStateChanged.Broadcast();
}

bool UAstraMissionRuntimeSubsystem::StartMission()
{
    if (!IsConfigured() || Outcome != EAstraRuntimeMissionOutcome::NotStarted)
    {
        return false;
    }

    if (Resources.Mass > ResourceLimits.Mass ||
        Resources.Energy < 0.0f || Resources.Oxygen <= 0.0f || Resources.TimeRemaining <= 0.0f)
    {
        Outcome = EAstraRuntimeMissionOutcome::Interrupted;
        OutcomeReason = TEXT("Configuration initiale invalide");
        OnMissionEnded.Broadcast(Outcome);
        return false;
    }

    Outcome = EAstraRuntimeMissionOutcome::InProgress;
    OutcomeReason = TEXT("Mission démarrée");
    OnRuntimeStateChanged.Broadcast();
    return true;
}

bool UAstraMissionRuntimeSubsystem::SpendResource(EAstraRuntimeResource Resource, float Amount)
{
    if (Outcome != EAstraRuntimeMissionOutcome::InProgress ||
        !AstraRuntimePrivate::IsFiniteNonNegative(Amount))
    {
        return false;
    }

    float* Value = FindResource(Resource);
    if (Value == nullptr || *Value < Amount)
    {
        return false;
    }

    *Value -= Amount;
    InterruptIfNecessary();
    OnRuntimeStateChanged.Broadcast();
    return Outcome == EAstraRuntimeMissionOutcome::InProgress;
}

bool UAstraMissionRuntimeSubsystem::ObservePoint(FName PointId, bool bHasSpectrometer, bool bHasCamera)
{
    if (Outcome != EAstraRuntimeMissionOutcome::InProgress)
    {
        return false;
    }

    FAstraRuntimeSciencePoint* Point = FindPoint(PointId);
    if (Point == nullptr || Point->bVisited || Point->PointId.IsNone())
    {
        return false;
    }
    if ((Point->bRequiresSpectrometer && !bHasSpectrometer) ||
        (Point->bRequiresCamera && !bHasCamera))
    {
        return false;
    }
    if (Resources.Data + Point->DataProduced > ResourceLimits.Data ||
        Resources.Mass + Point->SampleMass > ResourceLimits.Mass)
    {
        return false;
    }
    if (Resources.Energy < Point->EnergyCost || Resources.TimeRemaining < Point->TimeCost)
    {
        return false;
    }
    if (!SpendResource(EAstraRuntimeResource::Energy, Point->EnergyCost) ||
        !SpendResource(EAstraRuntimeResource::Time, Point->TimeCost))
    {
        return false;
    }

    Point->bVisited = true;
    Resources.Data += Point->DataProduced;
    Resources.Mass += Point->SampleMass;
    ScienceScore += Point->ScienceValue;
    if (Point->SampleMass > 0.0f)
    {
        ++CollectedSamples;
    }

    OnPointObserved.Broadcast(PointId);
    OnRuntimeStateChanged.Broadcast();
    return true;
}

bool UAstraMissionRuntimeSubsystem::FinishMission()
{
    if (Outcome != EAstraRuntimeMissionOutcome::InProgress)
    {
        return false;
    }

    int32 VisitedCount = 0;
    for (const FAstraRuntimeSciencePoint& Point : SciencePoints)
    {
        if (Point.bVisited)
        {
            ++VisitedCount;
        }
    }

    if (VisitedCount >= RequiredPoints && ScienceScore >= SuccessScienceThreshold)
    {
        Outcome = EAstraRuntimeMissionOutcome::Complete;
        OutcomeReason = TEXT("Objectifs scientifiques principaux accomplis");
    }
    else if (VisitedCount > 0 || ScienceScore > 0.0f)
    {
        Outcome = EAstraRuntimeMissionOutcome::Partial;
        OutcomeReason = TEXT("Une partie des objectifs a été accomplie");
    }
    else
    {
        Outcome = EAstraRuntimeMissionOutcome::ScientificFailure;
        OutcomeReason = TEXT("Données scientifiques insuffisantes");
    }

    OnMissionEnded.Broadcast(Outcome);
    OnRuntimeStateChanged.Broadcast();
    return true;
}

void UAstraMissionRuntimeSubsystem::ResetMission()
{
    Resources = InitialResources;
    ScienceScore = 0.0f;
    CollectedSamples = 0;
    Outcome = EAstraRuntimeMissionOutcome::NotStarted;
    OutcomeReason.Empty();
    for (FAstraRuntimeSciencePoint& Point : SciencePoints)
    {
        Point.bVisited = false;
    }
    OnRuntimeStateChanged.Broadcast();
}

bool UAstraMissionRuntimeSubsystem::IsMissionActive() const
{
    return Outcome == EAstraRuntimeMissionOutcome::InProgress;
}

FAstraRuntimeResourceState UAstraMissionRuntimeSubsystem::GetResourceState() const
{
    return Resources;
}

TArray<FAstraRuntimeSciencePoint> UAstraMissionRuntimeSubsystem::GetSciencePoints() const
{
    return SciencePoints;
}

FAstraRuntimeMissionReport UAstraMissionRuntimeSubsystem::GetMissionReport() const
{
    return BuildReport();
}

float* UAstraMissionRuntimeSubsystem::FindResource(EAstraRuntimeResource Resource)
{
    switch (Resource)
    {
    case EAstraRuntimeResource::Energy: return &Resources.Energy;
    case EAstraRuntimeResource::Oxygen: return &Resources.Oxygen;
    case EAstraRuntimeResource::Time: return &Resources.TimeRemaining;
    case EAstraRuntimeResource::Data: return &Resources.Data;
    case EAstraRuntimeResource::Communications: return &Resources.Communications;
    case EAstraRuntimeResource::Budget: return &Resources.Budget;
    case EAstraRuntimeResource::Mass: return &Resources.Mass;
    default: return nullptr;
    }
}

const float* UAstraMissionRuntimeSubsystem::FindResource(EAstraRuntimeResource Resource) const
{
    switch (Resource)
    {
    case EAstraRuntimeResource::Energy: return &Resources.Energy;
    case EAstraRuntimeResource::Oxygen: return &Resources.Oxygen;
    case EAstraRuntimeResource::Time: return &Resources.TimeRemaining;
    case EAstraRuntimeResource::Data: return &Resources.Data;
    case EAstraRuntimeResource::Communications: return &Resources.Communications;
    case EAstraRuntimeResource::Budget: return &Resources.Budget;
    case EAstraRuntimeResource::Mass: return &Resources.Mass;
    default: return nullptr;
    }
}

FAstraRuntimeSciencePoint* UAstraMissionRuntimeSubsystem::FindPoint(FName PointId)
{
    return SciencePoints.FindByPredicate([PointId](const FAstraRuntimeSciencePoint& Point)
    {
        return Point.PointId == PointId;
    });
}

void UAstraMissionRuntimeSubsystem::InterruptIfNecessary()
{
    if (Resources.Oxygen <= 0.0f)
    {
        Outcome = EAstraRuntimeMissionOutcome::Interrupted;
        OutcomeReason = TEXT("Réserve d'oxygène épuisée");
    }
    else if (Resources.TimeRemaining <= 0.0f)
    {
        Outcome = EAstraRuntimeMissionOutcome::Interrupted;
        OutcomeReason = TEXT("Temps de mission épuisé");
    }
}

FAstraRuntimeMissionReport UAstraMissionRuntimeSubsystem::BuildReport() const
{
    FAstraRuntimeMissionReport Report;
    Report.Outcome = Outcome;
    Report.MissionId = RuntimeMissionId;
    Report.ScienceScore = ScienceScore;
    Report.CollectedSamples = CollectedSamples;
    Report.Reason = OutcomeReason;
    for (const FAstraRuntimeSciencePoint& Point : SciencePoints)
    {
        if (Point.bVisited)
        {
            ++Report.VisitedPoints;
        }
    }
    return Report;
}

bool UAstraMissionRuntimeSubsystem::IsConfigured() const
{
    if (RuntimeMissionId.IsNone() || RequiredPoints <= 0 ||
        RequiredPoints > SciencePoints.Num() ||
        !AstraRuntimePrivate::IsFiniteNonNegative(SuccessScienceThreshold))
    {
        return false;
    }
    const bool bValidLimits = AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Energy) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Oxygen) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.TimeRemaining) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Data) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Communications) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Budget) &&
        AstraRuntimePrivate::IsFiniteNonNegative(ResourceLimits.Mass);
    const bool bValidInitials = AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Energy) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Oxygen) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.TimeRemaining) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Data) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Communications) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Budget) &&
        AstraRuntimePrivate::IsFiniteNonNegative(InitialResources.Mass);
    if (!bValidLimits || !bValidInitials ||
        InitialResources.Energy > ResourceLimits.Energy ||
        InitialResources.Oxygen > ResourceLimits.Oxygen ||
        InitialResources.TimeRemaining > ResourceLimits.TimeRemaining ||
        InitialResources.Data > ResourceLimits.Data ||
        InitialResources.Communications > ResourceLimits.Communications ||
        InitialResources.Budget > ResourceLimits.Budget ||
        InitialResources.Mass > ResourceLimits.Mass)
    {
        return false;
    }
    for (const FAstraRuntimeSciencePoint& Point : SciencePoints)
    {
        if (Point.PointId.IsNone() ||
            !AstraRuntimePrivate::IsFiniteNonNegative(Point.ScienceValue) ||
            !AstraRuntimePrivate::IsFiniteNonNegative(Point.DataProduced) ||
            !AstraRuntimePrivate::IsFiniteNonNegative(Point.EnergyCost) ||
            !AstraRuntimePrivate::IsFiniteNonNegative(Point.TimeCost) ||
            !AstraRuntimePrivate::IsFiniteNonNegative(Point.SampleMass) ||
            Point.DataProduced > ResourceLimits.Data ||
            Point.SampleMass > ResourceLimits.Mass ||
            Point.EnergyCost > InitialResources.Energy ||
            Point.TimeCost > InitialResources.TimeRemaining)
        {
            return false;
        }
    }
    return true;
}
