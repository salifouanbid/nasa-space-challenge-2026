#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AstraMissionRuntimeSubsystem.generated.h"

UENUM(BlueprintType)
enum class EAstraRuntimeResource : uint8
{
    Energy UMETA(DisplayName = "Énergie"),
    Oxygen UMETA(DisplayName = "Oxygène"),
    Time UMETA(DisplayName = "Temps"),
    Data UMETA(DisplayName = "Données"),
    Communications UMETA(DisplayName = "Communications"),
    Budget UMETA(DisplayName = "Budget"),
    Mass UMETA(DisplayName = "Masse")
};

UENUM(BlueprintType)
enum class EAstraRuntimeMissionOutcome : uint8
{
    NotStarted UMETA(DisplayName = "Non démarrée"),
    InProgress UMETA(DisplayName = "En cours"),
    Complete UMETA(DisplayName = "Réussie"),
    Partial UMETA(DisplayName = "Partielle"),
    ScientificFailure UMETA(DisplayName = "Échec scientifique"),
    Interrupted UMETA(DisplayName = "Interrompue")
};

USTRUCT(BlueprintType)
struct PROJETASTRA_API FAstraRuntimeResourceState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Energy = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Oxygen = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float TimeRemaining = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Data = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Communications = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Budget = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Mission|Ressources")
    float Mass = 0.0f;
};

USTRUCT(BlueprintType)
struct PROJETASTRA_API FAstraRuntimeSciencePoint
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    FName PointId = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    float ScienceValue = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    float DataProduced = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    float EnergyCost = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    float TimeCost = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    float SampleMass = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    bool bRequiresSpectrometer = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASTRA|Science")
    bool bRequiresCamera = false;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Science")
    bool bVisited = false;
};

USTRUCT(BlueprintType)
struct PROJETASTRA_API FAstraRuntimeMissionReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    EAstraRuntimeMissionOutcome Outcome = EAstraRuntimeMissionOutcome::NotStarted;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    FName MissionId = NAME_None;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    float ScienceScore = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    int32 VisitedPoints = 0;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    int32 CollectedSamples = 0;
    UPROPERTY(BlueprintReadOnly, Category = "ASTRA|Mission")
    FString Reason;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAstraRuntimeStateChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAstraRuntimePointObserved, FName, PointId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAstraRuntimeMissionEnded, EAstraRuntimeMissionOutcome, Outcome);

UCLASS(BlueprintType)
class PROJETASTRA_API UAstraMissionRuntimeSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    void ConfigureRuntimeMission(
        FName MissionId,
        const FAstraRuntimeResourceState& InitialResources,
        const FAstraRuntimeResourceState& ResourceLimits,
        int32 RequiredPoints,
        float SuccessScienceThreshold,
        const TArray<FAstraRuntimeSciencePoint>& SciencePoints);

    /** Initialise le scénario Gamma du dépôt avec ses deux points scientifiques. */
    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    void InitializeMission001RuntimeDefaults();

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    bool StartMission();

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    bool SpendResource(EAstraRuntimeResource Resource, float Amount);

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    bool ObservePoint(FName PointId, bool bHasSpectrometer, bool bHasCamera);

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    bool FinishMission();

    UFUNCTION(BlueprintCallable, Category = "ASTRA|Mission|Runtime")
    void ResetMission();

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission|Runtime")
    bool IsMissionActive() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission|Runtime")
    FAstraRuntimeResourceState GetResourceState() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission|Runtime")
    TArray<FAstraRuntimeSciencePoint> GetSciencePoints() const;

    UFUNCTION(BlueprintPure, Category = "ASTRA|Mission|Runtime")
    FAstraRuntimeMissionReport GetMissionReport() const;

    UPROPERTY(BlueprintAssignable, Category = "ASTRA|Mission|Runtime")
    FAstraRuntimeStateChanged OnRuntimeStateChanged;
    UPROPERTY(BlueprintAssignable, Category = "ASTRA|Mission|Runtime")
    FAstraRuntimePointObserved OnPointObserved;
    UPROPERTY(BlueprintAssignable, Category = "ASTRA|Mission|Runtime")
    FAstraRuntimeMissionEnded OnMissionEnded;

private:
    float* FindResource(EAstraRuntimeResource Resource);
    const float* FindResource(EAstraRuntimeResource Resource) const;
    FAstraRuntimeSciencePoint* FindPoint(FName PointId);
    void InterruptIfNecessary();
    FAstraRuntimeMissionReport BuildReport() const;
    bool IsConfigured() const;

    UPROPERTY(Transient)
    FName RuntimeMissionId = NAME_None;
    UPROPERTY(Transient)
    FAstraRuntimeResourceState InitialResources;
    UPROPERTY(Transient)
    FAstraRuntimeResourceState Resources;
    UPROPERTY(Transient)
    FAstraRuntimeResourceState ResourceLimits;
    UPROPERTY(Transient)
    TArray<FAstraRuntimeSciencePoint> SciencePoints;
    UPROPERTY(Transient)
    int32 RequiredPoints = 0;
    UPROPERTY(Transient)
    float SuccessScienceThreshold = 0.0f;
    UPROPERTY(Transient)
    float ScienceScore = 0.0f;
    UPROPERTY(Transient)
    int32 CollectedSamples = 0;
    UPROPERTY(Transient)
    EAstraRuntimeMissionOutcome Outcome = EAstraRuntimeMissionOutcome::NotStarted;
    UPROPERTY(Transient)
    FString OutcomeReason;
};
