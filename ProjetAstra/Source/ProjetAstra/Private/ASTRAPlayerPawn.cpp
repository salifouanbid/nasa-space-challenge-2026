#include "ASTRAPlayerPawn.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"


AASTRAPlayerPawn::AASTRAPlayerPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);


    // RESSOURCES 3D
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder")
    );

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(
        TEXT("/Engine/BasicShapes/Sphere.Sphere")
    );


    // CHASSIS
    Chassis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis"));
    Chassis->SetupAttachment(Root);

    if (Cube.Succeeded())
    {
        Chassis->SetStaticMesh(Cube.Object);
        Chassis->SetRelativeLocation(FVector(0, 0, 125));
        Chassis->SetRelativeScale3D(FVector(2.0, 1.2, 0.35));
    }


    // MODULE SUPERIEUR
    ModuleSuperieur = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("ModuleSuperieur")
    );

    ModuleSuperieur->SetupAttachment(Root);

    if (Cube.Succeeded())
    {
        ModuleSuperieur->SetStaticMesh(Cube.Object);
        ModuleSuperieur->SetRelativeLocation(FVector(0, 0, 175));
        ModuleSuperieur->SetRelativeScale3D(FVector(1.1, 0.8, 0.25));
    }


    // SIX ROUES
    UStaticMeshComponent* Roues[] = {
        Roue1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue1")),
        Roue2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue2")),
        Roue3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue3")),
        Roue4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue4")),
        Roue5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue5")),
        Roue6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roue6"))
    };

    FVector Positions[] = {
        FVector(-130, -115, 55),
        FVector(-130, 115, 55),
        FVector(0, -115, 55),
        FVector(0, 115, 55),
        FVector(130, -115, 55),
        FVector(130, 115, 55)
    };

    for (int32 i = 0; i < 6; i++)
    {
        Roues[i]->SetupAttachment(Root);

        if (Cylinder.Succeeded())
        {
            Roues[i]->SetStaticMesh(Cylinder.Object);
            Roues[i]->SetRelativeLocation(Positions[i]);

            // Orientation des roues
            Roues[i]->SetRelativeRotation(FRotator(0, 0, 90));

            // Dimensions des roues
            Roues[i]->SetRelativeScale3D(FVector(0.55, 0.55, 0.25));
        }
    }


    // ANTENNE
    Antenne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Antenne"));
    Antenne->SetupAttachment(Root);

    if (Cylinder.Succeeded())
    {
        Antenne->SetStaticMesh(Cylinder.Object);
        Antenne->SetRelativeLocation(FVector(0, 0, 250));
        Antenne->SetRelativeScale3D(FVector(0.06, 0.06, 0.55));
    }


    // CAMERA DU ROVER
    CameraRover = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("CameraRover")
    );

    CameraRover->SetupAttachment(Root);

    if (Sphere.Succeeded())
    {
        CameraRover->SetStaticMesh(Sphere.Object);
        CameraRover->SetRelativeLocation(FVector(110, 0, 190));
        CameraRover->SetRelativeScale3D(FVector(0.18, 0.18, 0.18));
    }


    // PANNEAU SOLAIRE
    PanneauSolaire = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("PanneauSolaire")
    );

    PanneauSolaire->SetupAttachment(Root);

    if (Cube.Succeeded())
    {
        PanneauSolaire->SetStaticMesh(Cube.Object);
        PanneauSolaire->SetRelativeLocation(FVector(-30, 160, 165));
        PanneauSolaire->SetRelativeScale3D(FVector(1.1, 0.8, 0.04));
    }


    // CAMERA DE SUIVI
    CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("CameraSpringArm")
    );

    CameraSpringArm->SetupAttachment(Root);
    CameraSpringArm->TargetArmLength = 600;
    CameraSpringArm->SetRelativeLocation(FVector(0, 0, 250));
    CameraSpringArm->SetRelativeRotation(FRotator(-15, 0, 0));

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraSpringArm);


    // POSSESSION AUTOMATIQUE
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}


// DEMARRAGE
void AASTRAPlayerPawn::BeginPlay()
{
    Super::BeginPlay();

    VitesseRover = 0.0f;
}


// MISE A JOUR
void AASTRAPlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


// DEPLACEMENT ET ROTATION DES SIX ROUES
void AASTRAPlayerPawn::Avancer(float Valeur)
{
    if (!GetWorld())
    {
        return;
    }

    const float DeltaTime = GetWorld()->GetDeltaSeconds();

    // Vitesse du rover
    VitesseRover = Valeur * 250.0f;

    // Déplacement du rover
    AddActorLocalOffset(
        FVector(VitesseRover * DeltaTime, 0.0f, 0.0f),
        true
    );

    // Rayon approximatif des roues en centimètres
    const float RayonRoue = 27.5f;

    // Calcul de l'angle de rotation des roues
    const float AngleRotation = FMath::RadiansToDegrees(
        (VitesseRover / RayonRoue) * DeltaTime
    );

    // Récupération des six roues
    UStaticMeshComponent* Roues[] = {
        Roue1, Roue2, Roue3,
        Roue4, Roue5, Roue6
    };

    // Rotation simultanée des six roues
    for (int32 i = 0; i < 6; i++)
    {
        if (IsValid(Roues[i]))
        {
            Roues[i]->AddLocalRotation(
                FRotator(0.0f, AngleRotation, 0.0f)
            );
        }
    }
}


// ROTATION DU ROVER
void AASTRAPlayerPawn::Tourner(float Valeur)
{
    if (!GetWorld())
    {
        return;
    }

    AddActorLocalRotation(
        FRotator(
            0.0f,
            Valeur * 80.0f * GetWorld()->GetDeltaSeconds(),
            0.0f
        )
    );
}


// COMMANDES DU JOUEUR
void AASTRAPlayerPawn::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent
)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(
        TEXT("MoveForward"),
        this,
        &AASTRAPlayerPawn::Avancer
    );

    PlayerInputComponent->BindAxis(
        TEXT("Turn"),
        this,
        &AASTRAPlayerPawn::Tourner
    );
}