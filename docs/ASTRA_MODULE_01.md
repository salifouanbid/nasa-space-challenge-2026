# ASTRA — Module 01 : Mission Design et Runtime

Le module de conception et de déroulement de mission est intégré au projet Unreal canonique situé dans `ProjetAstra/`.

## Emplacements intégrés

- Préparation de mission : `ProjetAstra/Source/ProjetAstra/Public/Mission/AstraMissionSubsystem.h`
- Runtime de mission : `ProjetAstra/Source/ProjetAstra/Public/Mission/AstraMissionRuntimeSubsystem.h`
- Implémentations : `ProjetAstra/Source/ProjetAstra/Private/Mission/`
- Scénario JSON de référence : `ProjetAstra/Content/ASTRA/Data/Missions/Mission_001_LunarSurvey.json`
- Projet Unreal : `ProjetAstra/ProjetAstra.uproject`

## Runtime Blueprint

Dans un Blueprint ayant accès au Game Instance :

1. Appeler `Get Game Instance` puis `Get Subsystem`.
2. Sélectionner `AstraMissionRuntimeSubsystem`.
3. Appeler `Initialize Mission 001 Runtime Defaults` pour charger le scénario Gamma prêt à tester.
4. Appeler `Start Mission`.
5. Appeler `Observe Point` avec `gamma-rim-observation` et `bHasCamera=true`.
6. Appeler `Observe Point` avec `gamma-shadowed-site` et `bHasSpectrometer=true`.
7. Appeler `Finish Mission`.
8. Lire `Get Mission Report` pour afficher le résultat.

Le runtime expose aussi `Configure Runtime Mission` pour charger Alpha, Beta ou un futur scénario depuis un widget, un Data Asset ou un lecteur JSON. Les ressources, les points scientifiques, les événements `OnRuntimeStateChanged`, `OnPointObserved` et `OnMissionEnded` sont accessibles aux Blueprints.

## Règles implémentées

Le runtime :

- vérifie la configuration initiale ;
- consomme l’énergie et le temps lors d’une observation ;
- refuse une observation si l’équipement requis est absent ;
- limite la capacité de données et la masse collectée ;
- interrompt la mission lorsque l’oxygène ou le temps sont épuisés ;
- calcule le score scientifique et le nombre d’échantillons ;
- produit les états `Complete`, `Partial`, `ScientificFailure` et `Interrupted`.

Les valeurs numériques sont des unités d’équilibrage de jeu. Elles ne sont pas des spécifications réelles de la NASA ou d'un système spatial certifié.

## Limites connues

La création des widgets UMG, le placement des points dans les niveaux et la compilation UE5.8 doivent encore être réalisés ou validés dans Unreal Editor sur Windows. La compilation Unreal n'a pas été exécutée dans l'environnement Linux d'intégration.
