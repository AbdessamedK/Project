# Portal – Simulation de Portails Dimensionnels en OpenGL

Ce projet implémente une simulation de portails dimensionnels en utilisant OpenGL. L'objectif est de permettre à un objet de traverser un portail en 3D, en manipulant les transformations géométriques et en gérant les intersections rayon-triangle pour assurer une transition fluide entre les dimensions.

## 🧠 Aspects Techniques

### 🔹 Manipulation d'OpenGL

- **Shaders GLSL** : Utilisation de shaders vertex et fragment pour le rendu des portails et des objets traversant.
- **Matrices de transformation** : Application de transformations pour simuler la traversée du portail.
- **Textures et mapping** : Gestion des textures pour représenter les surfaces des portails et des objets.

### 🔹 Intersection Rayon-Triangle

Pour déterminer si un rayon (représentant la trajectoire d'un objet) intersecte un triangle (représentant une surface du portail), l'algorithme de Möller–Trumbore est utilisé. Cet algorithme est efficace et largement utilisé dans les applications de ray tracing.

#### Algorithme de Möller–Trumbore

L'algorithme calcule l'intersection d'un rayon avec un triangle en 3D sans nécessiter de pré-calculs complexes. Il repose sur le calcul du produit vectoriel et du déterminant pour déterminer si le rayon intersecte le triangle et, le cas échéant, où se situe le point d'intersection.

Pour plus de détails sur cet algorithme, consultez la [documentation Wikipédia](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm).

### 🔹 Gestion des Portails

- **Détection d'entrée/sortie** : Détection précise de l'entrée et de la sortie à travers les portails en utilisant les intersections rayon-triangle.
- **Transformation des coordonnées** : Adaptation des coordonnées de l'objet traversant pour refléter sa nouvelle position et orientation dans la dimension cible.
