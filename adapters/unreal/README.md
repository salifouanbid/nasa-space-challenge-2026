# Adaptateur Unreal Engine 5.8

Ce dossier décrit le branchement du cœur portable `AstraCore` et du projet Unreal `ProjetAstra/` dans Unreal Engine 5.8. L’éditeur Unreal n’est pas installé dans l’environnement de génération, donc la compilation du module, l’ouverture des niveaux et la validation des Blueprints doivent être effectuées sur Windows avec UE5.8.

1. Ouvrir `ProjetAstra/ProjetAstra.uproject` avec Unreal Engine 5.8.
2. Le module Unreal `ProjetAstra` contient déjà le personnage, le rover, les interactions et `UAstraMissionSubsystem`.
3. Pour intégrer le cœur portable, ajouter `include/AstraCore` et `src/MissionCore.cpp` comme couche métier indépendante dans le module, sans recopier les règles dans les widgets.
4. Charger `data/scenarios/scenarios.json` depuis le répertoire de contenu empaqueté et convertir les entrées en `MissionConfig`/`SciencePoint`.
5. Exposer aux Blueprints les actions `StartMission`, `ObservePoint`, `SpendResource`, `FinishMission` et les événements `OnResourceChanged`, `OnDiscoveryRecorded`, `OnMissionEnded`.
6. Garder les widgets dans `UUserWidget` et ne pas dupliquer les calculs du cœur.

Le chargement JSON peut utiliser le module JSON natif d’Unreal. La logique métier reste sans types Unreal pour conserver la portabilité Unity.
