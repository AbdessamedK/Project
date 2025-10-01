# Traitement d’Images en C++

Ce projet a pour objectif d’implémenter différents **traitements d’images numériques** en C++. L’application permet de charger des images, de les manipuler et d’appliquer plusieurs types de traitement pour analyser et transformer les images.

---

## 🧠 Aspects Techniques

Le projet couvre plusieurs types de traitements d’images :

### 1. **Transformations de base**
- **Niveaux de gris** : Conversion d’une image couleur en niveaux de gris pour analyser l’intensité lumineuse.
- **Inversion des couleurs** : Inversion des pixels pour obtenir un effet négatif de l’image.
- **Seuil binaire** : Transformation d’une image en noir et blanc selon un seuil d’intensité.

### 2. **Filtrage et lissage**
- **Filtre moyenneur** : Réduction du bruit en remplaçant chaque pixel par la moyenne de ses voisins.
- **Filtre gaussien** : Lissage des images pour réduire le bruit tout en conservant les contours.
- **Filtre médian** : Suppression des pixels aberrants tout en conservant les contours.

### 3. **Détection de contours et gradients**
- **Détection de contours** : Identification des limites des objets présents dans l’image.
- **Gradient d’intensité** : Calcul des variations locales d’intensité pour détecter les bords.

### 4. **Analyse et manipulation avancée**
- **Histogrammes** : Analyse de la répartition des intensités pour effectuer des ajustements ou égalisations.
- **Morphologie** : Application d’opérations comme l’érosion et la dilatation pour traiter des formes dans l’image.
- **Transformations géométriques** : Rotation, translation et mise à l’échelle des images.

---
