# Données NASA utilisées par ASTRA

Cette première base utilise des données et constats scientifiques publics. Les valeurs de mission (énergie, oxygène, budget, score et coûts d’action) sont des valeurs pédagogiques paramétrables et ne sont pas présentées comme des spécifications de matériel spatial réel.

## 1. Terrain du scénario Gamma

Le scénario Gamma est fondé sur la mosaïque **South Pole LOLA DEM Mosaic** publiée par le Planetary Geology, Geophysics and Geochemistry Laboratory de la NASA/GSFC : couverture 87–90° sud, GeoTIFF de hauteur de surface à 5 m/pixel, coordonnées polaires stéréographiques et référence MOON_ME/DE421. La page indique aussi une erreur RMS typique de 30 à 55 cm pour la mosaïque.

Source : https://pgda.gsfc.nasa.gov/products/81

Dans le dépôt, ces faits sont représentés dans `data/scenarios/scenarios.json` par le profil `south_pole_shadowed_craters` et par les contraintes pédagogiques du scénario. Le fichier de 3,3 Go n’est pas copié dans Git pour ne pas rendre le dépôt inutilement lourd ; l’adaptateur moteur devra télécharger ou convertir un sous-ensemble validé du GeoTIFF avant de produire le niveau 3D.

## 2. Référence topographique complémentaire

La fiche NASA du **SLDEM2015** décrit un modèle numérique de terrain fusionnant les mesures altimétriques LOLA de la mission LRO avec les données de caméra terrain SELENE. Elle indique une couverture globale entre 60° sud et 60° nord, une résolution publiée jusqu’à 512 pixels par degré et une précision verticale typique de 3 à 4 m pour le produit combiné.

Source : https://pgda.gsfc.nasa.gov/products/54

Cette source est conservée comme base potentielle pour Alpha et Beta, tandis que Gamma utilise la mosaïque polaire LOLA dédiée.

## 3. Contexte scientifique de la glace lunaire

NASA décrit des indices de glace d’eau dans des régions polaires ombragées et rappelle que les résultats sont des observations scientifiques, pas une garantie que chaque zone contient de la glace. Le scénario Gamma suit cette précaution : le point `possible_ice_site` est une cible d’observation et non une récompense garantie.

Source : https://science.nasa.gov/moon/moon-water-and-ices/

## 4. Visualisation et noms de reliefs

La visualisation NASA du terrain du pôle Sud de la Lune précise que ses couleurs encodent l’altitude et qu’elle s’appuie sur des DEM LRO/LOLA et SLDEM2015. Elle est utilisée comme référence de présentation et de vocabulaire, pas comme texture copiée dans le dépôt.

Source : https://svs.gsfc.nasa.gov/5127

## 5. Règle de traçabilité

Toute future texture, hauteur, coordonnée ou zone scientifique importée dans le jeu devra ajouter son URL source, sa date de téléchargement, son format, sa licence ou ses conditions d’utilisation, ainsi que la transformation appliquée. Les données scientifiques et les simplifications de gameplay resteront séparées dans les fichiers de configuration.
