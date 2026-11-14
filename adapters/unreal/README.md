# Adaptateur Unreal Engine 4.8

Ce dossier décrit le branchement du cœur `AstraCore` dans un projet Unreal 4.8. L’éditeur Unreal n’est pas installé dans l’environnement de génération, donc aucun fichier `.uproject` ou Blueprint binaire n’est fabriqué ici.

1. Copier `include/AstraCore` et `src/MissionCore.cpp` dans un module C++ du projet.
2. Ajouter le chemin d’inclusion au fichier `*.Build.cs`.
3. Créer une classe `UAstraMissionSubsystem` ou un `BP_MissionManager` qui possède une instance de `astra::MissionCore`.
4. Charger `data/scenarios/scenarios.json` depuis le répertoire de contenu empaqueté et convertir les entrées en `MissionConfig`/`SciencePoint`.
5. Exposer aux Blueprints les actions `StartMission`, `ObservePoint`, `SpendResource`, `FinishMission` et les événements `OnResourceChanged`, `OnDiscoveryRecorded`, `OnMissionEnded`.
6. Garder les widgets dans `UUserWidget` et ne pas dupliquer les calculs du cœur.

Le chargement JSON peut utiliser le module JSON natif d’Unreal. La logique métier reste sans types Unreal pour conserver la portabilité Unity.
