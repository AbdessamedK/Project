# SurvivorHero!


## But du jeu

SurvivorHero! est un jeu 2D solo combinant action, survie et arcade ! Le joueur doit résister à des vagues d'ennemies dans chaque monde pour ensuite avoir une chance
d'affronter le boss du monde.

## Utilisation

Pour se déplacer, il faut utiliser Z pour avancer, Q aller à gauche, D aller à droite et enfin S pour descendre. Pour pouvoir tirer des flèches, le personnage ne doit pas bouger, c'est-a-dire qu'aucune touche de déplacement doit etre préssé. A la fin d'un monde, un pouvoir ou un atout vous sera proposé. Pour continuer le jeu, il vous faudra selectionné une des options proposés. Une fois les monstres tué, pour le monde 1 et 3, vous avez juste à vous diriger vers la porte de la map.

## Développement du jeu

Le jeu est codé en C++ et en utlisant SDL2 comme bibliothèque graphique. 

## Code et organisation des fichiers

Ce code est construit de manière à avoir les fonctionalités de base regroupées dans des classes noyau; 
puis d'avoir une surcouche pour l'affichage : texte/console, SDL2, Qt, OpenGl, etc.

L'organisation :

- **survivorhero/src/core**   : les classes et modules de base du jeu. Ces fichiers représentent le noyeau du jeu.
- **survivorhero/src/txt**   : les classes s'occupant de faire tourner le jeu (core) et de l'afficher sur la console en texte.
- **survivorhero/src/sdl**   : les classes s'occupant de faire tourner le jeu (core) et de l'afficher avec SDL2.
- **survivorhero/data**      : les assets (image, sons, fichiers textes ...).
- **survivorhero/obj**       : les différents fichiers objets (.o) du jeu.
- **survivorhero/bin**       : les différents éxécutable du jeu.
- **Makefile**              : la création des différents éxécutables.

Pour la documentation du code, voir la rubrique "documentation du code" plus bas.


## Pour compiler

### Préambule : si vous voulez un Linux sous Windows, installez WSL ! 

Voir la page de l'UE : https://perso.liris.cnrs.fr/alexandre.meyer/public_html/www/doku.php?id=lifap4#wsl_pour_faire_tourner_un_linux_ultra_leger_sous_windows

C'est léger, pratique, rapide et suffisant pour de nombreuses applications.
Windows/WSL est exactement comme une machine Linux avec des drivers minimalistes, 
donc la gestion de certains périphériques commes la carte son ou l'accélération 3D peut ne pas marcher mais pour ce cours ca peut être suffisant.
Il faut parcontre que vous ayez installé XMing pour pouvoir ouvrir des fen^tres comme expliqué sur la page du cours. 
Valgrind marche aussi !


### Pour installer SDL2
Pour installer SDL2 sous Linux ou avec WSL/Windows
``` sudo apt install python3 valgrind doxygen libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev imagemagick ```

### Sous Linux/MacOS
Lancer la commande make est vous placant dans le bon répertoire puis vous avez le choix entre 
- **bin/SDLjeu** : Pour la version graphique du jeu.
- **bin/survivor_hero_txt** : Pour la version txt dans un terminal

### Sous Windows
- installer MinGW ou CodeBlocks 20 (qui installe MinGW)
- ajouter le chemin vers le compilateur dans la variable PATH. Dans Paramètres, rechercher PATH, puis Modifier variable d'environnements.
  Et ajouter le chemin, si CB : `C:\Program Files\CodeBlocks\MinGW\bin` et si MinGW `C:\MinGW\bin`
  Tester en lancant un CMDPrompt et taper `g++ --version`
- La version simple : ouvrer le terminal intégré à VisualCode avec le menu ou `ctrl+ù`puis lancer `mingw32-make -f Makefile`
- la version intégré `ctrl+shift+B`
- Dans VisualCode : Ctrl+Shift+P et cherche `task`
