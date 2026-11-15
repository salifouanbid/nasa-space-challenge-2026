# ASTRA — Module 01 : Mission Design System

Le module de conception de mission est intégré au projet Unreal canonique situé dans `ProjetAstra/`.

## Emplacements intégrés

- Code C++ public : `ProjetAstra/Source/ProjetAstra/Public/Mission/`
- Code C++ privé : `ProjetAstra/Source/ProjetAstra/Private/Mission/`
- Scénario JSON : `ProjetAstra/Content/ASTRA/Data/Missions/Mission_001_LunarSurvey.json`
- Projet Unreal : `ProjetAstra/ProjetAstra.uproject`

## Fonctionnalités

Le système :

- stocke les contraintes d'une mission ;
- contient un catalogue d'équipements ;
- ajoute et retire les équipements choisis par le joueur ;
- calcule masse, coût, énergie, carburant et capacité de communication ;
- vérifie les dépendances entre équipements ;
- renvoie les blocages et avertissements de validation ;
- expose les fonctions et structures à Blueprint ;
- fournit `InitializeMission001Defaults()` pour charger le scénario lunaire de démonstration.

Les valeurs numériques sont des unités d'équilibrage de jeu. Elles ne sont pas des spécifications réelles de la NASA ou d'un système spatial certifié.

## Utilisation Blueprint

1. Appeler `Get Game Instance`.
2. Appeler `Get Subsystem` et sélectionner `AstraMissionSubsystem`.
3. Appeler `Initialize Mission 001 Defaults`.
4. Appeler `Get Available Equipment` pour obtenir le catalogue.
5. Utiliser `Add Equipment By Id` avec les identifiants tels que `EQ_CAMERA`, `EQ_BATTERY` et `EQ_ROVER`.
6. Appeler `Evaluate Configuration` pour lire les totaux, marges, avertissements et blocages.
7. Autoriser la validation uniquement si `bCanLaunchMission` vaut `true`.

Pour la configuration complète de démonstration, les sept équipements du catalogue produisent les valeurs attendues : masse 120, coût 177, marge énergétique 25, marge carburant 20 et communication 80.

## Vérification

La compilation Unreal doit être effectuée avec Unreal Engine 5.8 sur une machine disposant de l'éditeur et de l'outil `Build.bat`. Elle n'a pas été exécutée dans l'environnement Linux d'intégration.
