#pragma once

#include "CoreMinimal.h"
#include "AstraMissionTypes.generated.h"

UENUM(BlueprintType)
enum class EAstraEquipmentCategory : uint8
{
    ScientificInstrument UMETA(DisplayName = "Instrument scientifique"),
    PowerSystem          UMETA(DisplayName = "SystÃ¨me Ã©nergÃ©tique"),
    Mobility             UMETA(DisplayName = "MobilitÃ©"),
    Communication        UMETA(DisplayName = "Communication"),
    Support              UMETA(DisplayName = "Support")
};

/** Valeurs d'Ã©quilibrage du prototype (unitÃ©s de jeu), pas des spÃ©cifications NASA. */
USTRUCT(BlueprintType)
struct PROJETASTRA_API FAstraEquipmentDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ã‰quipement")
    FName EquipmentId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ã‰quipement")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ã‰quipement", meta = (MultiLine = "true"))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ã‰quipement")
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Ã‰quipement")
    TArray<FName> RequiredEquipmentIds;
};

USTRUCT(BlueprintType)
struct PROJETASTRA_API FAstraMissionConstraints
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
struct PROJETASTRA_API FAstraMissionEvaluation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    bool bCanLaunchMission = false;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalMassKg = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float RemainingMassKg = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalCostCredits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float RemainingBudgetCredits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalEnergySupplyUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalEnergyDemandUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float EnergyMarginUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalFuelSupplyUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float TotalFuelDemandUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    float FuelMarginUnits = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    int32 CommunicationPoints = 0;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    int32 RequiredCommunicationPoints = 0;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    TArray<FText> BlockingIssues;

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Ã‰valuation")
    TArray<FText> Warnings;
};
