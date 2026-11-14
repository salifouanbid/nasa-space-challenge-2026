# Architecture portable ASTRA

## Principe

Le projet sépare le modèle de mission de l’affichage et de l’entrée utilisateur. `AstraCore` contient uniquement du C++ standard C++11 : ressources, observations, objectifs et rapport final. Il ne dépend ni d’Unreal, ni de Unity, ni d’une bibliothèque graphique.

Les scénarios et textes vivent dans `data/` en JSON. Les valeurs qui décrivent le gameplay sont donc modifiables sans réécrire les règles.

## Intégration Unreal Engine 4.8

Créer un module C++ `AstraCore` dans le projet Unreal et ajouter `include/` et `src/` au module. Le futur `BP_MissionManager` appelle une façade Unreal qui traduit les événements Blueprint vers `MissionCore`. Les widgets proposés par le cahier des charges (`WBP_MissionPrep`, `WBP_ExplorationHUD`, `WBP_ScientificMap`, `WBP_Lab`, `WBP_MissionReport`) ne doivent pas contenir les règles de ressources.

L’environnement actuel ne contient pas Unreal Engine 4.8 : la compilation de l’éditeur, des niveaux et des Blueprints devra être faite sur une machine équipée de la version demandée. Le cœur portable est volontairement écrit dans un sous-ensemble compatible avec les compilateurs utilisés par les anciennes versions d’Unreal.

## Intégration Unity

Créer un wrapper C# ou un plugin natif qui expose les mêmes opérations : `Start`, `Observe`, `Spend`, `Finish` et `Report`. Les scripts Unity doivent charger les mêmes JSON et mapper les événements vers les mêmes identifiants de points scientifiques.

## Collaboration de la première livraison

La collaboration est locale et simulée : journal de mission, rôles affichés et décisions observables sur le même poste. Aucun serveur, compte, matchmaking ou réplication réseau n’est requis dans cette version.

## Limite importante

La portabilité concerne les règles, les données et les contrats d’événements. Les scènes 3D, matériaux, Pawn/Character, véhicules, widgets et contrôleurs restent spécifiques à chaque moteur.
