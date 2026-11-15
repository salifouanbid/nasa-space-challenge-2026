#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AstraMissionTypes.h"
#include "AstraMissionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAstraMissionConfigurationChanged);

/** Service central de conception et de validation des missions ASTRA. */
UCLASS(BlueprintType)
class PROJETASTRA_API UAstraMissionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission")
    void ConfigureMission(const FAstraMissionConstraints& NewConstraints);

    /** Charge le scÃ©nario de dÃ©monstration M001 avec un jeu d'Ã©quipements cohÃ©rent. */
    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission")
    bool InitializeMission001Defaults();

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission")
    bool IsMissionConfigured() const;

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Ã‰quipement")
    bool RegisterEquipment(const FAstraEquipmentDefinition& Equipment);

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Ã‰quipement")
    void ClearEquipmentCatalogue();

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Ã‰quipement")
    bool AddEquipmentById(FName EquipmentId);

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Ã‰quipement")
    bool RemoveEquipmentById(FName EquipmentId);

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission")
    void ResetMissionDesign();

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission")
    FAstraMissionEvaluation EvaluateConfiguration() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission")
    bool CanLaunchMission() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Ã‰quipement")
    TArray<FAstraEquipmentDefinition> GetAvailableEquipment() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Ã‰quipement")
    TArray<FAstraEquipmentDefinition> GetSelectedEquipment() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission")
    FAstraMissionConstraints GetMissionConstraints() const;

    UPROPERTY(BlueprintAssignable, Category = "ASTRA|Mission")
    FAstraMissionConfigurationChanged OnConfigurationChanged;

private:
    UPROPERTY(Transient)
    TArray<FAstraEquipmentDefinition> AvailableEquipment;

    UPROPERTY(Transient)
    TArray<FAstraEquipmentDefinition> SelectedEquipment;

    UPROPERTY(Transient)
    FAstraMissionConstraints MissionConstraints;

    UPROPERTY(Transient)
    bool bMissionConfigured = false;

    bool IsEquipmentDefinitionValid(const FAstraEquipmentDefinition& Equipment) const;
};
