# Traitement d’Images et Détection de Formes en C++

Ce projet implémente plusieurs techniques avancées de **traitement d’images numériques** en C++. L’objectif est de détecter des formes géométriques dans des images, d’appliquer des méthodes robustes aux bruits et aux données aberrantes, et de manipuler les images à l’aide de C++.

---

## 🧠 Aspects Techniques

### 1. **Détection de droites avec la Transformée de Hough**

- **Description** : Utilisation de la transformée de Hough pour détecter des droites dans une image.  
- **Principe** : Chaque pixel de l’image contribue à l’espace des paramètres (ρ, θ) pour identifier des droites potentielles.  
- **Application** : Détection des contours, lignes de bord ou trajectoires rectilignes dans les images.

### 2. **Détection de cercles avec la Transformée de Hough**

- **Description** : Détection de cercles en utilisant la transformée de Hough pour cercles.  
- **Principe** : Recherche des ensembles de points qui peuvent former un cercle avec un centre et un rayon précis.  
- **Application** : Reconnaissance de formes circulaires, détection de boutons, roues ou repères ronds dans l’image.

### 3. **RANSAC (RANdom SAmple Consensus)**

- **Description** : Algorithme robuste pour estimer des modèles géométriques à partir de données contenant des outliers.  
- **Principe** : 
  1. Sélection aléatoire d’un sous-ensemble de points.  
  2. Estimation d’un modèle (ex : droite, cercle).  
  3. Comptage des points qui correspondent au modèle (inliers).  
  4. Répétition pour trouver le modèle avec le plus grand nombre d’inliers.  
- **Application** : Détection fiable de droites ou cercles même en présence de bruit ou de points aberrants.

### 4. **Autres traitements d’images**

- Conversion en niveaux de gris pour simplifier le traitement.  
- Seuillage binaire pour isoler les formes d’intérêt.  
- Floutage ou filtrage pour réduire le bruit avant détection.

---
