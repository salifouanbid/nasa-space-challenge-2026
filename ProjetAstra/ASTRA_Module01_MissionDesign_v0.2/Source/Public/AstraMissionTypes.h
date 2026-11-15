#pragma once

#include "CoreMinimal.h"
#include "AstraMissionTypes.generated.h"

UENUM(BlueprintType)
enum class EAstraEquipmentCategory : uint8
{
    ScientificInstrument UMETA(DisplayName = "Instrument scientifique"),
    PowerSystem          UMETA(DisplayName = "Système énergétique"),
    Mobility             UMETA(DisplayName = "Mobilité"),
    Communication        UMETA(DisplayName = "Communication"),
    Support              UMETA(DisplayName = "Support")
};

/** Valeurs d'équilibrage du prototype (unités de jeu), pas des spécifications NASA. */
USTRUCT(BlueprintType)
struct __API__ FAstraEquipmentDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Équipement")
    FName EquipmentId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Équipement")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Équipement", meta = (MultiLine = "true"))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Équipement")
    EAstraEquipmentCategory Category = EAstraEquipmentCategory::Support;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float MassKg = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float CostCredits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float EnergySupplyUnits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float EnergyDemandUnits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float FuelSupplyUnits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0.0"))
    float FuelDemandUnits = 0.0f;

    /** Indice abstrait de communication, et non un bilan de liaison radio. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ressources", meta = (ClampMin = "0"))
    int32 CommunicationPoints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Équipement")
    TArray<FName> RequiredEquipmentIds;
};

USTRUCT(BlueprintType)
struct __API__ FAstraMissionConstraints
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission")
    FName MissionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission")
    FText MissionTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission", meta = (MultiLine = "true"))
    FText ScientificObjective;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Contraintes", meta = (ClampMin = "0.01"))
    float MaxPayloadMassKg = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Contraintes", meta = (ClampMin = "0.01"))
    float MaxBudgetCredits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Contraintes", meta = (ClampMin = "0.0"))
    float MinimumEnergyMarginUnits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Contraintes", meta = (ClampMin = "0.0"))
    float MinimumFuelMarginUnits = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Contraintes", meta = (ClampMin = "0"))
    int32 MinimumCommunicationPoints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Avertissements", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float MassWarningRatio = 0.9f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Avertissements", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float BudgetWarningRatio = 0.9f;
};

USTRUCT(BlueprintType)
struct __API__ FAstraMissionEvaluation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    bool bCanLaunchMission = false;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalMassKg = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float RemainingMassKg = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalCostCredits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float RemainingBudgetCredits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalEnergySupplyUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalEnergyDemandUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float EnergyMarginUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalFuelSupplyUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float TotalFuelDemandUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    float FuelMarginUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    int32 CommunicationPoints = 0;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    int32 RequiredCommunicationPoints = 0;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    TArray<FText> BlockingIssues;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Évaluation")
    TArray<FText> Warnings;
};
