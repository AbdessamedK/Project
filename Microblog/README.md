# Microblog

## Description

Microblog est une application de microblogging développée avec Flask, offrant une interface simple pour partager des messages courts. Ce projet illustre l'application de **design patterns** pour une architecture modulaire et maintenable, ainsi qu'une gestion de projet agile pour une livraison efficace.

## Design Patterns

### 1. **Factory Pattern**
Le **Factory Pattern** est utilisé pour créer des objets sans spécifier la classe exacte de l'objet à créer. Dans ce projet, il est appliqué pour l'initialisation des composants de l'application, permettant une extensibilité et une maintenance facilitées.

### 2. **Singleton Pattern**
Le **Singleton Pattern** garantit qu'une classe n'a qu'une seule instance et fournit un point d'accès global à cette instance. Il est utilisé pour la gestion de la configuration de l'application, assurant une cohérence dans l'ensemble du projet.

### 3. **Observer Pattern**
Le **Observer Pattern** est implémenté pour gérer les notifications en temps réel. Lorsqu'un utilisateur publie un nouveau message, tous les abonnés sont automatiquement informés, offrant une expérience interactive.

## Gestion de Projet

Le développement de Microblog a suivi une approche agile, avec les étapes suivantes :

- **Planification** : Définition des fonctionnalités clés et des objectifs du projet.
- **Développement** : Implémentation des fonctionnalités en s'appuyant sur les design patterns pour assurer une architecture robuste.
- **Tests** : Mise en place de tests unitaires pour valider le bon fonctionnement de chaque composant.
- **Déploiement** : Publication de l'application sur un serveur de production pour un accès public.
