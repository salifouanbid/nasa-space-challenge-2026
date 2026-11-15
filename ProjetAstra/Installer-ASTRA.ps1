$ErrorActionPreference = 'Stop'

function Write-Utf8File {
    param([Parameter(Mandatory=$true)][string]$Path, [Parameter(Mandatory=$true)][string]$Content)
    $Directory = Split-Path -Parent $Path
    if (-not (Test-Path $Directory)) { New-Item -ItemType Directory -Path $Directory -Force | Out-Null }
    $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($Path, $Content, $Utf8NoBom)
}

$ProjectRoot = (Get-Location).Path
$UProjects = @(Get-ChildItem -Path $ProjectRoot -Filter '*.uproject' -File)
if ($UProjects.Count -ne 1) {
    throw "Ouvre PowerShell dans le dossier qui contient exactement un fichier .uproject. Fichiers .uproject trouvés : $($UProjects.Count)"
}

$UProjectFile = $UProjects[0].FullName
$ProjectName = [System.IO.Path]::GetFileNameWithoutExtension($UProjectFile)
$ProjectJson = Get-Content -LiteralPath $UProjectFile -Raw | ConvertFrom-Json

$RuntimeModule = $null
if ($ProjectJson.Modules) {
    $RuntimeModule = @($ProjectJson.Modules | Where-Object { $_.Type -eq 'Runtime' } | Select-Object -First 1)[0]
}
$ModuleName = if ($RuntimeModule -and $RuntimeModule.Name) { [string]$RuntimeModule.Name } else { $ProjectName }
if ($ModuleName -notmatch '^[A-Za-z][A-Za-z0-9_]*$') {
    throw "Nom de module non valide pour C++ : $ModuleName. Le nom doit commencer par une lettre et ne contenir que lettres, chiffres et tirets bas."
}
$ApiMacro = ($ModuleName.ToUpperInvariant() -replace '[^A-Z0-9_]', '') + '_API'

Write-Host "Projet détecté : $ProjectName" -ForegroundColor Cyan
Write-Host "Module C++ utilisé : $ModuleName" -ForegroundColor Cyan
Write-Host "Dossier : $ProjectRoot" -ForegroundColor Cyan

# Add a Runtime module declaration if the .uproject does not already contain one.
$Modules = @()
if ($ProjectJson.Modules) { $Modules = @($ProjectJson.Modules) }
$HasRuntimeModule = @($Modules | Where-Object { $_.Name -eq $ModuleName -and $_.Type -eq 'Runtime' }).Count -gt 0
if (-not $HasRuntimeModule) {
    $Modules += [pscustomobject]@{ Name = $ModuleName; Type = 'Runtime'; LoadingPhase = 'Default' }
    if ($ProjectJson.PSObject.Properties.Name -contains 'Modules') {
        $ProjectJson.Modules = $Modules
    } else {
        $ProjectJson | Add-Member -MemberType NoteProperty -Name Modules -Value $Modules
    }
    $JsonText = $ProjectJson | ConvertTo-Json -Depth 100
    Write-Utf8File -Path $UProjectFile -Content ($JsonText + "`n")
}

$SourceRoot = Join-Path $ProjectRoot 'Source'
$ModuleRoot = Join-Path $SourceRoot $ModuleName
$PublicRoot = Join-Path $ModuleRoot 'Public'
$PrivateRoot = Join-Path $ModuleRoot 'Private'

# Create the content and source folder layout.
$Folders = @(
    $PublicRoot,
    $PrivateRoot,
    (Join-Path $PublicRoot 'Characters'),
    (Join-Path $PublicRoot 'Core'),
    (Join-Path $PublicRoot 'Interaction'),
    (Join-Path $PrivateRoot 'Characters'),
    (Join-Path $PrivateRoot 'Core'),
    (Join-Path $PrivateRoot 'Interaction'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Blueprints'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Characters'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Environments'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Materials'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Meshes'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Planets'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Vehicles'),
    (Join-Path $ProjectRoot 'Content/ASTRA/UI'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Audio'),
    (Join-Path $ProjectRoot 'Content/ASTRA/Maps'),
    (Join-Path $ProjectRoot 'Config')
)
foreach ($Folder in $Folders) { New-Item -ItemType Directory -Path $Folder -Force | Out-Null }

$BuildCs = @'
using UnrealBuildTool;

public class __MODULE__ : ModuleRules
{
    public __MODULE__(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore"
        });
    }
}
'@.Replace('__MODULE__', $ModuleName)
Write-Utf8File (Join-Path $ModuleRoot ($ModuleName + '.Build.cs')) $BuildCs

$GameTarget = @'
using UnrealBuildTool;
using System.Collections.Generic;

public class __MODULE__Target : TargetRules
{
    public __MODULE__Target(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("__MODULE__");
    }
}
'@.Replace('__MODULE__', $ModuleName)
Write-Utf8File (Join-Path $SourceRoot ($ModuleName + '.Target.cs')) $GameTarget

$EditorTarget = @'
using UnrealBuildTool;
using System.Collections.Generic;

public class __MODULE__EditorTarget : TargetRules
{
    public __MODULE__EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("__MODULE__");
    }
}
'@.Replace('__MODULE__', $ModuleName)
Write-Utf8File (Join-Path $SourceRoot ($ModuleName + 'Editor.Target.cs')) $EditorTarget

$ModuleCpp = @'
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, __MODULE__, "__MODULE__");
'@.Replace('__MODULE__', $ModuleName)
Write-Utf8File (Join-Path $PrivateRoot ($ModuleName + '.cpp')) $ModuleCpp

$GameModeH = @'
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AstraGameMode.generated.h"

UCLASS(Blueprintable)
class __API__ AAstraGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAstraGameMode();
};
'@.Replace('__API__', $ApiMacro)
Write-Utf8File (Join-Path $PublicRoot 'AstraGameMode.h') $GameModeH

$GameModeCpp = @'
#include "AstraGameMode.h"
#include "AstraExplorerCharacter.h"

AAstraGameMode::AAstraGameMode()
{
    DefaultPawnClass = AAstraExplorerCharacter::StaticClass();
}
'@
Write-Utf8File (Join-Path $PrivateRoot 'AstraGameMode.cpp') $GameModeCpp

$CharacterH = @'
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AstraExplorerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class __API__ AAstraExplorerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAstraExplorerCharacter();

protected:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Interact();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;
};
'@.Replace('__API__', $ApiMacro)
Write-Utf8File (Join-Path $PublicRoot 'AstraExplorerCharacter.h') $CharacterH

$CharacterCpp = @'
#include "AstraExplorerCharacter.h"
#include "AstraInteractableActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Math/RotationMatrix.h"

AAstraExplorerCharacter::AAstraExplorerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 500.0f;
    GetCharacterMovement()->AirControl = 0.30f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 350.0f;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void AAstraExplorerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAstraExplorerCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAstraExplorerCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAstraExplorerCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAstraExplorerCharacter::LookUp);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AAstraExplorerCharacter::Interact);
}

void AAstraExplorerCharacter::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AAstraExplorerCharacter::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AAstraExplorerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AAstraExplorerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AAstraExplorerCharacter::Interact()
{
    if (!FollowCamera || !GetWorld())
    {
        return;
    }

    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * 600.0f;
    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ASTRA_Interaction), false, this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, QueryParams))
    {
        if (AAstraInteractableActor* Target = Cast<AAstraInteractableActor>(Hit.GetActor()))
        {
            Target->Interact(this);
        }
    }
}
'@
Write-Utf8File (Join-Path $PrivateRoot 'AstraExplorerCharacter.cpp') $CharacterCpp

$InteractableH = @'
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstraInteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class __API__ AAstraInteractableActor : public AActor
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
'@.Replace('__API__', $ApiMacro)
Write-Utf8File (Join-Path $PublicRoot 'AstraInteractableActor.h') $InteractableH

$InteractableCpp = @'
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

    DisplayName = TEXT("Échantillon scientifique");
}

void AAstraInteractableActor::Interact(AActor* InteractingActor)
{
    const FString Message = FString::Printf(TEXT("ASTRA : échantillon analysé — %s"), *DisplayName);
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, Message);
    }
}
'@
Write-Utf8File (Join-Path $PrivateRoot 'AstraInteractableActor.cpp') $InteractableCpp

# Set the C++ GameMode as the project default, while leaving the user's current map untouched.
$DefaultEngineIni = Join-Path $ProjectRoot 'Config/DefaultEngine.ini'
$EngineConfig = ''
if (Test-Path $DefaultEngineIni) { $EngineConfig = Get-Content -LiteralPath $DefaultEngineIni -Raw }
$GameModeLine = "GlobalDefaultGameMode=/Script/$ModuleName.AstraGameMode"
if ($EngineConfig -notmatch [regex]::Escape($GameModeLine)) {
    $EngineConfig = $EngineConfig.TrimEnd() + "`r`n`r`n[/Script/EngineSettings.GameMapsSettings]`r`n$GameModeLine`r`n"
    Write-Utf8File $DefaultEngineIni $EngineConfig
}

# Legacy keyboard/mouse input mappings used by this starter character.
$DefaultInputIni = Join-Path $ProjectRoot 'Config/DefaultInput.ini'
$InputLines = @(
    '[/Script/Engine.InputSettings]',
    '+AxisMappings=(AxisName="MoveForward",Scale=1.000000,Key=W)',
    '+AxisMappings=(AxisName="MoveForward",Scale=-1.000000,Key=S)',
    '+AxisMappings=(AxisName="MoveRight",Scale=1.000000,Key=D)',
    '+AxisMappings=(AxisName="MoveRight",Scale=-1.000000,Key=A)',
    '+AxisMappings=(AxisName="Turn",Scale=1.000000,Key=MouseX)',
    '+AxisMappings=(AxisName="LookUp",Scale=-1.000000,Key=MouseY)',
    '+ActionMappings=(ActionName="Jump",Key=SpaceBar)',
    '+ActionMappings=(ActionName="Interact",Key=E)'
)
$InputConfig = ''
if (Test-Path $DefaultInputIni) { $InputConfig = Get-Content -LiteralPath $DefaultInputIni -Raw }
foreach ($Line in $InputLines) {
    if ($InputConfig -notmatch [regex]::Escape($Line)) { $InputConfig += $Line + "`r`n" }
}
Write-Utf8File $DefaultInputIni $InputConfig

$Readme = @'
ASTRA — démarrage du socle C++

Projet : __PROJECT__
Module : __MODULE__

Ce script a créé le socle C++ et l'arborescence de contenu. Il ne crée pas encore les modèles 3D,
les animations, les planètes détaillées, les sons ou les interfaces finales : ces éléments doivent
être fabriqués/importés dans Unreal au fil des étapes.

Commandes du personnage de base :
- W, A, S, D : déplacement
- Souris : orientation de la caméra
- Espace : saut
- E : interaction avec un acteur AAstraInteractableActor

Étapes après exécution :
1. Fermer Unreal Editor.
2. Clic droit sur le fichier .uproject > Generate Visual Studio project files.
3. Ouvrir le .sln généré.
4. Choisir Development Editor et Win64, puis Build Solution.
5. Relancer le .uproject.
6. Dans le niveau, ajouter un Player Start et un sol (Cube étiré ou Landscape).
7. Dans Project Settings > Maps & Modes, vérifier que AstraGameMode est le GameMode par défaut.
8. Pour tester l'interaction, ajouter un acteur C++ AstraInteractableActor au niveau, devant le Player Start.

Le personnage est pour l'instant une capsule Unreal sans modèle d'astronaute. Le modèle 3D et les animations
seront ajoutés plus tard via un Blueprint dérivé de AstraExplorerCharacter.
'@
$Readme = $Readme.Replace('__PROJECT__', $ProjectName).Replace('__MODULE__', $ModuleName)
Write-Utf8File (Join-Path $ProjectRoot 'ASTRA-DEMARRAGE.txt') $Readme

Write-Host ''
Write-Host 'Socle ASTRA créé avec succès.' -ForegroundColor Green
Write-Host "Fichiers C++ : $ModuleRoot" -ForegroundColor Green
Write-Host "Configuration : $(Join-Path $ProjectRoot 'Config')" -ForegroundColor Green
Write-Host "Guide : $(Join-Path $ProjectRoot 'ASTRA-DEMARRAGE.txt')" -ForegroundColor Green
Write-Host ''
Write-Host 'IMPORTANT : ferme Unreal Editor avant de générer les fichiers Visual Studio et de compiler.' -ForegroundColor Yellow
