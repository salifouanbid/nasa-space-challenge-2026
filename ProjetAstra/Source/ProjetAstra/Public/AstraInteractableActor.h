#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstraInteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class PROJETASTRA_API AAstraInteractableActor : public AActor
{
    GENERATED_BODY()

public:
    AAstraInteractableActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ASTRA")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ASTRA")
    FString DisplayName;

    UFUNCTION(BlueprintCallable, Category="ASTRA|Interaction")
    virtual void Interact(AActor* InteractingActor);
};