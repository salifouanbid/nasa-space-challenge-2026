#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASTRAPlayerPawn.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;

UCLASS()
class PROJETASTRA_API AASTRAPlayerPawn : public APawn
{
    GENERATED_BODY()

public:

    AASTRAPlayerPawn();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(
        UInputComponent* PlayerInputComponent
    ) override;


protected:

    virtual void BeginPlay() override;

    // VITESSE DU ROVER
    float VitesseRover = 0.0f;


    // STRUCTURE PRINCIPALE
    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Chassis;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* ModuleSuperieur;


    // EQUIPEMENTS
    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* PanneauSolaire;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Antenne;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* CameraRover;


    // SYSTEME DE ROUES
    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue1;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue2;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue3;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue4;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue5;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Rover")
    UStaticMeshComponent* Roue6;


    // CAMERA DE SUIVI
    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Camera")
    USpringArmComponent* CameraSpringArm;

    UPROPERTY(VisibleAnywhere, Category = "ASTRA|Camera")
    UCameraComponent* Camera;


    // COMMANDES DU ROVER
    void Avancer(float Valeur);

    void Tourner(float Valeur);
};