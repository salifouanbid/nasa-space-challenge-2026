#include "Mission/AstraMissionSubsystem.h"

namespace AstraMissionPrivate
{
    bool IsNonNegativeFinite(const float Value)
    {
        return FMath::IsFinite(Value) && Value >= 0.0f;
    }

    bool ContainsId(const TArray<FAstraEquipmentDefinition>& Items, const FName Id)
    {
        return Items.ContainsByPredicate([Id](const FAstraEquipmentDefinition& Item)
        {
            return Item.EquipmentId == Id;
        });
    }
}

void UAstraMissionSubsystem::ConfigureMission(const FAstraMissionConstraints& NewConstraints)
{
    MissionConstraints = NewConstraints;
    bMissionConfigured =
        !MissionConstraints.MissionId.IsNone() &&
        FMath::IsFinite(MissionConstraints.MaxPayloadMassKg) && MissionConstraints.MaxPayloadMassKg > 0.0f &&
        FMath::IsFinite(MissionConstraints.MaxBudgetCredits) && MissionConstraints.MaxBudgetCredits > 0.0f &&
        AstraMissionPrivate::IsNonNegativeFinite(MissionConstraints.MinimumEnergyMarginUnits) &&
        AstraMissionPrivate::IsNonNegativeFinite(MissionConstraints.MinimumFuelMarginUnits) &&
        MissionConstraints.MinimumCommunicationPoints >= 0 &&
        FMath::IsFinite(MissionConstraints.MassWarningRatio) && MissionConstraints.MassWarningRatio > 0.0f && MissionConstraints.MassWarningRatio <= 1.0f &&
        FMath::IsFinite(MissionConstraints.BudgetWarningRatio) && MissionConstraints.BudgetWarningRatio > 0.0f && MissionConstraints.BudgetWarningRatio <= 1.0f;

    OnConfigurationChanged.Broadcast();
}

bool UAstraMissionSubsystem::InitializeMission001Defaults()
{
    ClearEquipmentCatalogue();
    SelectedEquipment.Reset();

    FAstraMissionConstraints Constraints;
    Constraints.MissionId = TEXT("ASTRA_M001_LUNAR_SURVEY");
    Constraints.MissionTitle = FText::FromString(TEXT("Mission 001 â€” Reconnaissance et collecte lunaire"));
    Constraints.ScientificObjective = FText::FromString(TEXT("Ã‰tudier une petite zone lunaire et rÃ©cupÃ©rer trois Ã©lÃ©ments scientifiques : rÃ©golithe, fragment rocheux et Ã©chantillon minÃ©ral."));
    Constraints.MaxPayloadMassKg = 120.0f;
    Constraints.MaxBudgetCredits = 190.0f; // 177 crÃ©dits pour le catalogue complet de dÃ©monstration.
    Constraints.MinimumEnergyMarginUnits = 15.0f;
    Constraints.MinimumFuelMarginUnits = 10.0f;
    Constraints.MinimumCommunicationPoints = 50;
    Constraints.MassWarningRatio = 0.90f;
    Constraints.BudgetWarningRatio = 0.90f;
    ConfigureMission(Constraints);

    auto MakeEquipment = [](const FName Id, const TCHAR* Name, const TCHAR* Description,
        const EAstraEquipmentCategory Category, const float Mass, const float Cost,
        const float EnergySupply, const float EnergyDemand, const float FuelSupply,
        const float FuelDemand, const int32 Communication, const TArray<FName>& Requirements)
    {
        FAstraEquipmentDefinition Item;
        Item.EquipmentId = Id;
        Item.DisplayName = FText::FromString(Name);
        Item.Description = FText::FromString(Description);
        Item.Category = Category;
        Item.MassKg = Mass;
        Item.CostCredits = Cost;
        Item.EnergySupplyUnits = EnergySupply;
        Item.EnergyDemandUnits = EnergyDemand;
        Item.FuelSupplyUnits = FuelSupply;
        Item.FuelDemandUnits = FuelDemand;
        Item.CommunicationPoints = Communication;
        Item.RequiredEquipmentIds = Requirements;
        return Item;
    };

    const TArray<FName> NoRequirements;
    const TArray<FName> BatteryRequirement = { FName(TEXT("EQ_BATTERY")) };

    const bool bAllRegistered =
        RegisterEquipment(MakeEquipment(TEXT("EQ_CAMERA"), TEXT("CamÃ©ra panoramique"), TEXT("Imagerie gÃ©nÃ©rale de la zone d'Ã©tude."), EAstraEquipmentCategory::ScientificInstrument, 4.0f, 12.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0, NoRequirements)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_SPECTROMETER"), TEXT("SpectromÃ¨tre portable"), TEXT("Analyse minÃ©ralogique simplifiÃ©e."), EAstraEquipmentCategory::ScientificInstrument, 12.0f, 30.0f, 0.0f, 18.0f, 0.0f, 0.0f, 0, BatteryRequirement)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_DRILL"), TEXT("Carottier simplifiÃ©"), TEXT("PrÃ©lÃ¨vement d'un Ã©chantillon superficiel."), EAstraEquipmentCategory::ScientificInstrument, 18.0f, 25.0f, 0.0f, 20.0f, 0.0f, 0.0f, 0, BatteryRequirement)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_BATTERY"), TEXT("Module batterie"), TEXT("Stockage d'Ã©nergie pour les instruments."), EAstraEquipmentCategory::PowerSystem, 14.0f, 15.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0, NoRequirements)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_SOLAR_ARRAY"), TEXT("Panneau solaire dÃ©ployable"), TEXT("Apport Ã©nergÃ©tique simplifiÃ© pour la mission."), EAstraEquipmentCategory::PowerSystem, 20.0f, 22.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0, NoRequirements)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_HIGH_GAIN_RADIO"), TEXT("Antenne de communication"), TEXT("Maintien d'une liaison de communication de gameplay."), EAstraEquipmentCategory::Communication, 8.0f, 18.0f, 0.0f, 8.0f, 0.0f, 0.0f, 80, NoRequirements)) &&
        RegisterEquipment(MakeEquipment(TEXT("EQ_ROVER"), TEXT("Rover d'exploration lÃ©ger"), TEXT("MobilitÃ© de surface et rÃ©serve de carburant abstraite."), EAstraEquipmentCategory::Mobility, 44.0f, 55.0f, 0.0f, 8.0f, 100.0f, 80.0f, 0, NoRequirements));

    OnConfigurationChanged.Broadcast();
    return bMissionConfigured && bAllRegistered;
}

bool UAstraMissionSubsystem::IsMissionConfigured() const
{
    return bMissionConfigured;
}

bool UAstraMissionSubsystem::IsEquipmentDefinitionValid(const FAstraEquipmentDefinition& Equipment) const
{
    return Equipment.EquipmentId != NAME_None &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.MassKg) &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.CostCredits) &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.EnergySupplyUnits) &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.EnergyDemandUnits) &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.FuelSupplyUnits) &&
        AstraMissionPrivate::IsNonNegativeFinite(Equipment.FuelDemandUnits) &&
        Equipment.CommunicationPoints >= 0;
}

bool UAstraMissionSubsystem::RegisterEquipment(const FAstraEquipmentDefinition& Equipment)
{
    if (!IsEquipmentDefinitionValid(Equipment) || AstraMissionPrivate::ContainsId(AvailableEquipment, Equipment.EquipmentId))
    {
        return false;
    }

    AvailableEquipment.Add(Equipment);
    OnConfigurationChanged.Broadcast();
    return true;
}

void UAstraMissionSubsystem::ClearEquipmentCatalogue()
{
    AvailableEquipment.Reset();
    SelectedEquipment.Reset();
    OnConfigurationChanged.Broadcast();
}

bool UAstraMissionSubsystem::AddEquipmentById(const FName EquipmentId)
{
    if (EquipmentId.IsNone() || AstraMissionPrivate::ContainsId(SelectedEquipment, EquipmentId))
    {
        return false;
    }

    const FAstraEquipmentDefinition* Found = AvailableEquipment.FindByPredicate([EquipmentId](const FAstraEquipmentDefinition& Item)
    {
        return Item.EquipmentId == EquipmentId;
    });

    if (!Found)
    {
        return false;
    }

    SelectedEquipment.Add(*Found);
    OnConfigurationChanged.Broadcast();
    return true;
}

bool UAstraMissionSubsystem::RemoveEquipmentById(const FName EquipmentId)
{
    const int32 RemovedCount = SelectedEquipment.RemoveAll([EquipmentId](const FAstraEquipmentDefinition& Item)
    {
        return Item.EquipmentId == EquipmentId;
    });

    if (RemovedCount > 0)
    {
        OnConfigurationChanged.Broadcast();
        return true;
    }
    return false;
}

void UAstraMissionSubsystem::ResetMissionDesign()
{
    SelectedEquipment.Reset();
    OnConfigurationChanged.Broadcast();
}

FAstraMissionEvaluation UAstraMissionSubsystem::EvaluateConfiguration() const
{
    FAstraMissionEvaluation Result;
    Result.RequiredCommunicationPoints = MissionConstraints.MinimumCommunicationPoints;

    if (!bMissionConfigured)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("Les contraintes de mission ne sont pas configurees ou sont invalides.")));
        return Result;
    }

    if (SelectedEquipment.IsEmpty())
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("Selectionnez au moins un equipement.")));
    }

    TSet<FName> SelectedIds;
    for (const FAstraEquipmentDefinition& Equipment : SelectedEquipment)
    {
        SelectedIds.Add(Equipment.EquipmentId);
        Result.TotalMassKg += Equipment.MassKg;
        Result.TotalCostCredits += Equipment.CostCredits;
        Result.TotalEnergySupplyUnits += Equipment.EnergySupplyUnits;
        Result.TotalEnergyDemandUnits += Equipment.EnergyDemandUnits;
        Result.TotalFuelSupplyUnits += Equipment.FuelSupplyUnits;
        Result.TotalFuelDemandUnits += Equipment.FuelDemandUnits;
        Result.CommunicationPoints += Equipment.CommunicationPoints;
    }

    Result.RemainingMassKg = MissionConstraints.MaxPayloadMassKg - Result.TotalMassKg;
    Result.RemainingBudgetCredits = MissionConstraints.MaxBudgetCredits - Result.TotalCostCredits;
    Result.EnergyMarginUnits = Result.TotalEnergySupplyUnits - Result.TotalEnergyDemandUnits;
    Result.FuelMarginUnits = Result.TotalFuelSupplyUnits - Result.TotalFuelDemandUnits;

    if (Result.TotalMassKg > MissionConstraints.MaxPayloadMassKg + KINDA_SMALL_NUMBER)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("La masse depasse la capacite de la mission.")));
    }
    else if (Result.TotalMassKg >= MissionConstraints.MaxPayloadMassKg * MissionConstraints.MassWarningRatio)
    {
        Result.Warnings.Add(FText::FromString(TEXT("La masse approche de la limite autorisee.")));
    }

    if (Result.TotalCostCredits > MissionConstraints.MaxBudgetCredits + KINDA_SMALL_NUMBER)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("Le cout depasse le budget disponible.")));
    }
    else if (Result.TotalCostCredits >= MissionConstraints.MaxBudgetCredits * MissionConstraints.BudgetWarningRatio)
    {
        Result.Warnings.Add(FText::FromString(TEXT("Le cout approche de la limite budgetaire.")));
    }

    if (Result.EnergyMarginUnits + KINDA_SMALL_NUMBER < MissionConstraints.MinimumEnergyMarginUnits)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("La marge energetique est insuffisante.")));
    }
    if (Result.FuelMarginUnits + KINDA_SMALL_NUMBER < MissionConstraints.MinimumFuelMarginUnits)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("La reserve de carburant est insuffisante.")));
    }
    if (Result.CommunicationPoints < MissionConstraints.MinimumCommunicationPoints)
    {
        Result.BlockingIssues.Add(FText::FromString(TEXT("La capacite de communication est insuffisante.")));
    }

    for (const FAstraEquipmentDefinition& Equipment : SelectedEquipment)
    {
        for (const FName RequiredId : Equipment.RequiredEquipmentIds)
        {
            if (!SelectedIds.Contains(RequiredId))
            {
                Result.BlockingIssues.Add(FText::Format(
                    FText::FromString(TEXT("{0} necessite l'equipement {1}.")),
                    Equipment.DisplayName,
                    FText::FromName(RequiredId)));
            }
        }
    }

    Result.bCanLaunchMission = Result.BlockingIssues.IsEmpty();
    return Result;
}

bool UAstraMissionSubsystem::CanLaunchMission() const
{
    return EvaluateConfiguration().bCanLaunchMission;
}

TArray<FAstraEquipmentDefinition> UAstraMissionSubsystem::GetAvailableEquipment() const
{
    return AvailableEquipment;
}

TArray<FAstraEquipmentDefinition> UAstraMissionSubsystem::GetSelectedEquipment() const
{
    return SelectedEquipment;
}

FAstraMissionConstraints UAstraMissionSubsystem::GetMissionConstraints() const
{
    return MissionConstraints;
}
