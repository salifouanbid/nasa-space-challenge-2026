# ASTRA — Module 01 : Mission Design System (v0.2)

Cette version ajoute le noyau C++ du concepteur de mission et un scénario de démonstration M001 prêt à initialiser depuis Blueprint.

## Ce que le module fait

- stocke les contraintes d'une mission ;
- contient un catalogue d'équipements ;
- ajoute et retire les équipements choisis par le joueur ;
- calcule masse, coût, énergie, carburant et capacité de communication ;
- vérifie les dépendances entre équipements ;
- renvoie les blocages et avertissements de validation ;
- expose les fonctions et structures à Blueprint ;
- fournit `InitializeMission001Defaults()` pour charger le scénario lunaire de démonstration.

Les valeurs numériques sont des unités d'équilibrage de jeu. Elles ne sont pas des spécifications réelles de la NASA ou d'un système spatial certifié.

## Installation dans le projet existant

1. **Fermer complètement Unreal Editor.**
2. Télécharger le ZIP et extraire le dossier `ASTRA_Module01_MissionDesign_v0.2` directement dans le dossier du projet, à côté de `ProjetAstra.uproject`.
3. Ouvrir PowerShell dans le dossier du projet qui contient `ProjetAstra.uproject`.
4. Exécuter une seule fois :

```powershell
& ".\ASTRA_Module01_MissionDesign_v0.2\INSTALLER-ASTRA-MISSION-DESIGN.ps1"
```

Le script détecte automatiquement le module Runtime dans le `.uproject`, calcule le bon macro d'export (`PROJETASTRA_API` dans le projet actuel), crée les dossiers `Public\Mission` et `Private\Mission`, puis installe le JSON dans `Content\ASTRA\Data\Missions`.

Les fichiers portant le même nom sont sauvegardés dans `Saved\ASTRA_Backups\MissionDesign_v0.2` avant remplacement. Le script ne modifie pas `.uproject`, `Build.cs` ou les fichiers Target.cs.

## Compilation

Après l'installation, en gardant Unreal Editor fermé, lancer depuis PowerShell dans le dossier du projet :

```powershell
& "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" ProjetAstraEditor Win64 Development "-Project=$((Resolve-Path .\ProjetAstra.uproject).Path)" -WaitMutex
```

Ne considérer le module comme intégré qu'après un résultat `Result: Succeeded`.

## Première utilisation dans Blueprint

Après compilation et réouverture du projet :

1. Ouvrir un Blueprint qui a accès au Game Instance (par exemple le Level Blueprint pour un premier test, puis un futur menu de conception).
2. Appeler `Get Game Instance`.
3. Depuis le résultat, appeler `Get Subsystem` et choisir `AstraMissionSubsystem`.
4. Appeler `Initialize Mission 001 Defaults`.
5. Appeler `Get Available Equipment` pour obtenir les sept éléments du catalogue.
6. Appeler `Add Equipment By Id` avec les identifiants comme `EQ_CAMERA`, `EQ_BATTERY`, `EQ_ROVER`.
7. Appeler `Evaluate Configuration` pour lire les totaux, marges, avertissements et blocages.
8. Autoriser le bouton de validation uniquement si `bCanLaunchMission` vaut `true`.

Pour valider la configuration complète de démonstration, ajouter les sept identifiants du catalogue. Le résultat attendu est : masse 120, coût 177, marge énergétique 25, marge carburant 20, communication 80. La mission passe avec un avertissement de masse proche de la limite.

## Équipements de démonstration

- `EQ_CAMERA` — caméra panoramique ;
- `EQ_SPECTROMETER` — spectromètre portable (nécessite `EQ_BATTERY`) ;
- `EQ_DRILL` — carottier simplifié (nécessite `EQ_BATTERY`) ;
- `EQ_BATTERY` — module batterie ;
- `EQ_SOLAR_ARRAY` — panneau solaire ;
- `EQ_HIGH_GAIN_RADIO` — antenne de communication ;
- `EQ_ROVER` — rover simplifié.

## Ce qui n'est pas encore fait

Ce module est le noyau logique de la conception et de la validation de ressources. Il ne fournit pas encore le menu UMG, l'interface graphique de sélection, le lanceur, la simulation orbitale, l'astronaute, le rover 3D ou les opérations scientifiques jouables. Ces éléments seront ajoutés dans les jalons suivants.
