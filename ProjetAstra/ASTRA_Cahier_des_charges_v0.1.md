# ASTRA — Cahier des charges initial

**Nom du jeu :** ASTRA — Universal Space Exploration  
**Nom technique du projet Unreal :** `ProjetAstra`  
**Moteur ciblé :** Unreal Engine 5.8  
**Document :** Version 0.1 — cadrage initial

---

## 1. Vision du jeu

ASTRA est un jeu d'exploration spatiale à vocation scientifique et éducative. Le joueur incarne un astronaute qui prépare des expéditions depuis une base spatiale, explore différents corps célestes à pied ou en rover, observe des phénomènes, collecte des échantillons et accomplit des missions.

La direction artistique vise un rendu principalement réaliste, avec environ **20 % d'éléments de science-fiction**. La science-fiction sert l'immersion et l'interface, mais ne doit pas remplacer la logique scientifique du jeu.

La Lune sera le premier terrain jouable. Elle servira à valider les systèmes qui seront ensuite réutilisés pour d'autres destinations : Mars, astéroïdes, lunes et autres corps célestes.

## 2. Principes de conception

1. **Exploration avant tout :** le joueur doit pouvoir observer, se déplacer et découvrir son environnement.
2. **Science accessible :** chaque activité scientifique doit expliquer simplement ce que le joueur observe et pourquoi c'est intéressant.
3. **Progression cohérente :** les missions débloquent progressivement des équipements, des destinations et des connaissances.
4. **Technologie crédible :** les outils et véhicules doivent avoir une fonction compréhensible dans l'expédition.
5. **Architecture réutilisable :** les systèmes communs ne doivent pas être recodés pour chaque astre.

## 3. Boucle de jeu principale

1. Le joueur arrive à la base de commandement.
2. Il consulte les objectifs et les informations scientifiques de la mission.
3. Il choisit son équipement et prépare le rover si nécessaire.
4. Il rejoint la zone d'exploration.
5. Il observe des points d'intérêt et interagit avec certains objets.
6. Il collecte ou analyse des échantillons lorsque la mission le demande.
7. Il retourne à la base ou transmet les données de mission.
8. Le jeu évalue les objectifs accomplis, met à jour le journal scientifique et débloque la suite.

## 4. Périmètre du premier prototype jouable — Lune

Le premier prototype doit rester limité et testable. Il comprendra :

- une petite base ou zone de départ lunaire ;
- un astronaute contrôlable à la troisième personne ;
- une caméra et des commandes de déplacement ;
- un terrain lunaire de taille maîtrisée ;
- quelques roches et points d'intérêt ;
- un système d'interaction simple ;
- une première collecte d'échantillon ;
- un journal ou panneau affichant les découvertes ;
- une mission courte avec objectifs et condition de réussite.

Le rover sera ajouté après validation du déplacement à pied et des interactions. Cette séparation réduit les risques et facilite les tests.

## 5. Systèmes prévus pour les versions suivantes

### 5.1 Base de commandement
- Tableau de mission.
- Préparation des expéditions.
- Sélection de destination lorsque plusieurs zones seront disponibles.
- Accès au journal scientifique et aux résultats.

### 5.2 Astronaute
- Déplacement à pied.
- Course et saut seulement si adaptés à la gravité et au gameplay.
- Interaction avec les objets.
- Équipement visuel et animations.

### 5.3 Rover
- Entrée et sortie du véhicule.
- Conduite et caméra dédiée.
- Énergie et état du véhicule, introduits progressivement.
- Transport d'échantillons et d'équipements.

### 5.4 Exploration scientifique
- Points d'intérêt identifiables.
- Observation et description.
- Collecte conditionnée par les objectifs de mission.
- Analyse des échantillons à la base.
- Journal scientifique qui conserve les découvertes.

### 5.5 Missions et progression
- Objectifs principaux et secondaires.
- Suivi de progression.
- Validation des résultats.
- Déblocage progressif de nouvelles missions et destinations.

### 5.6 Interface
- Menu principal.
- HUD discret pendant l'exploration.
- Indicateur d'interaction.
- Carte ou radar lorsque nécessaire.
- Journal de mission et journal scientifique.
- Écran de résultats.

## 6. Organisation technique cible

Le projet conserve le nom technique `ProjetAstra`. Les classes C++ du module portent donc ce préfixe technique. L'organisation suivante est une cible évolutive ; les dossiers ne seront créés que lorsqu'ils seront utilisés.

```text
ProjetAstra/
├── Config/
├── Content/
│   └── ASTRA/
│       ├── Maps/
│       ├── Blueprints/
│       ├── Characters/
│       ├── Vehicles/
│       ├── Environments/
│       ├── Materials/
│       ├── Meshes/
│       ├── Textures/
│       ├── UI/
│       ├── Audio/
│       └── Data/
├── Source/
│   └── ProjetAstra/
│       ├── Public/
│       │   ├── Core/
│       │   ├── Characters/
│       │   ├── Interaction/
│       │   ├── Exploration/
│       │   ├── Missions/
│       │   ├── Vehicles/
│       │   ├── Science/
│       │   └── UI/
│       └── Private/
│           ├── Core/
│           ├── Characters/
│           ├── Interaction/
│           ├── Exploration/
│           ├── Missions/
│           ├── Vehicles/
│           ├── Science/
│           └── UI/
└── ProjetAstra.uproject
```

### Règles techniques

- C++ pour les systèmes fondamentaux et les comportements qui doivent être robustes.
- Blueprints pour l'assemblage d'acteurs, les réglages de contenu et le prototypage visuel.
- Python d'éditeur uniquement pour automatiser des tâches dans l'éditeur ; il ne remplace pas le code runtime du jeu.
- Les données de mission et les descriptions scientifiques doivent être séparées autant que possible de la logique du jeu.
- Chaque nouveau système doit être compilé et testé avant d'en ajouter un autre.
- Aucun script ne doit être ajouté sans chemin précis, dépendances et procédure de vérification.

## 7. Phases de développement

| Phase | Livrable vérifiable |
|---|---|
| 0. Environnement | Unreal, compilateur C++ et SDK détectés |
| 1. Socle technique | Projet C++ qui compile et démarre |
| 2. Démarrage du jeu | GameMode, personnage et carte de test |
| 3. Déplacement | Astronaute contrôlable et caméra fonctionnelle |
| 4. Interaction | Un objet test peut être ciblé et inspecté |
| 5. Terrain lunaire | Petite zone d'exploration jouable |
| 6. Mission 1 | Objectifs, progression et réussite |
| 7. Science | Collecte, analyse simple et journal |
| 8. Rover | Entrée, conduite, sortie et tests |
| 9. Base | Préparation et consultation des missions |
| 10. Extension | Nouveaux sites et corps célestes |
| 11. Finition | Audio, effets, optimisation, accessibilité et tests |

## 8. Critères d'acceptation du prototype lunaire

Le prototype sera considéré comme jouable lorsque :

- le projet compile sans erreur ;
- la carte de départ s'ouvre sans erreur bloquante ;
- le joueur apparaît au point de départ ;
- les commandes de déplacement et la caméra fonctionnent ;
- le joueur peut atteindre un point d'intérêt ;
- l'interaction affiche une information ;
- l'échantillon demandé peut être collecté ;
- le jeu met à jour la progression de la mission ;
- la mission peut être terminée et son résultat affiché.

## 9. Éléments volontairement reportés

Pour éviter de rendre le premier prototype trop complexe, les éléments suivants ne sont pas prioritaires :

- simulation complète de l'orbite et de la mécanique céleste ;
- monde ouvert à l'échelle réelle ;
- multijoueur en ligne ;
- économie complexe et fabrication avancée ;
- survie détaillée avec de nombreux paramètres ;
- toutes les planètes dès la première version.

Ces fonctionnalités pourront être étudiées après la validation du prototype de base.

## 10. État actuel

- Le projet Unreal `ProjetAstra` existe.
- Un premier socle de fichiers C++ a été généré par le script d'installation initial.
- La génération des fichiers de projet a échoué parce que l'environnement de compilation Visual C++ n'était pas correctement détecté et que le format de projet par défaut était mal configuré.
- L'installation/configuration de Visual Studio et des outils C++ est en cours.
- Aucune fonctionnalité du jeu ne doit être considérée comme terminée tant qu'elle n'a pas été compilée et testée dans Unreal Engine.

---

**Règle de développement :** avancer par petites étapes vérifiables. Ne pas passer à un système suivant tant que le système courant n'a pas été compilé, testé et sauvegardé.
