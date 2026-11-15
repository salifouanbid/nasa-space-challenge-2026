#include "AstraInteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

AAstraInteractableActor::AAstraInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    SetRootComponent(MeshComponent);
    MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMesh.Object);
        MeshComponent->SetRelativeScale3D(FVector(0.5f));
    }

    DisplayName = TEXT("Ã‰chantillon scientifique");
}

void AAstraInteractableActor::Interact(AActor* InteractingActor)
{
    const FString Message = FString::Printf(TEXT("ASTRA : Ã©chantillon analysÃ© â€” %s"), *DisplayName);
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
    }
}