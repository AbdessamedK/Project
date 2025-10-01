# 🤖 RobotEV3 – Vision & Navigation Autonome

Ce projet implémente un système de navigation autonome pour un robot **LEGO Mindstorms EV3**, basé sur la **vision par ordinateur**.  
L’objectif est de permettre au robot de reconnaître et d’analyser son environnement (une arène), afin d’adapter son comportement en temps réel.

---

## 🧠 Aspects techniques
Le projet combine robotique et traitement d’image :

- **Homographie** : transformation de perspective utilisée pour reconstruire une vue "vue de dessus" de l’arène à partir d’une image de caméra.
- **Segmentation et analyse d’image** : détection de zones d’intérêt (bordures, obstacles, zones cibles), suivi de lignes ou de repères visuels.
- **Computer Vision (OpenCV)** :
  - Calibration caméra → correction des distorsions optiques.
  - Extraction de contours et filtrage d’objets.
  - Suivi en temps réel pour fournir des commandes au robot.
- **Contrôle du robot EV3** :
  - Communication entre la vision (PC) et la brique EV3.
  - Envoi d’ordres moteurs pour déplacement, rotation, arrêt.
  - Ajustements basés sur la perception de l’environnement.

---
