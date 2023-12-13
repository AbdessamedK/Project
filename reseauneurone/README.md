# Exploration des Réseaux de Neurones

Bienvenue dans le README de notre projet d'exploration des réseaux de neurones ! Ce projet vise à acquérir une compréhension approfondie de l'utilisation des réseaux de neurones en explorant les cadres TensorFlow et PyTorch. Nous avons abordé divers aspects, de la reconnaissance de chiffres à la détection d'émotions faciales. Voici un aperçu des principales sections du projet :

1. Reconnaissance de Chiffres avec TensorFlow (MNIST)

Dans cette section, nous avons créé un modèle avec TensorFlow pour la reconnaissance de chiffres en utilisant le célèbre jeu de données MNIST. Ce modèle a servi pour faire une application on l'on écrit un chiffre et on demande au modèle si il reconnait.

2. Autoencodeur avec TensorFlow

En utilisant le modèle précédent comme point de départ, nous avons mis en œuvre un autoencodeur. Cette approche nous a permis de comprendre les principes de la compression et de la reconstruction des données à l'aide des réseaux de neurones.

3. Reconnaissance de Dessins avec TensorFlow (QuickDraw)

Dans cette section, nous avons créé un modèle avec TensorFlow pour la reconnaissance de dessins en utilisant le jeu de données de QuickDraw. Nous avons fait une application similaire à celle de la reconnaissance des chiffres, mais pour les dessins cette fois-ci. Seuls quelques objets sont reconnus car nous n'avons entrainés notre modèle que sur une petite partie de QuickDraw, il reconnait les ballons de basket, la tour Eiffel, des pommes, et quelques autres objets.

4. Classification d'Images avec TensorFlow

Nous avons développé un modèle TensorFlow pour classer des images de chiens, chats, avions, etc., en utilisant le jeu de données CIFAR10. 
En parallèle, nous avons créé un modèle similaire pour la classification de cellules malades ou saines dans un autre jeu de données dédié.

5. Application d'Émotion Faciale avec PyTorch

Dans cette phase, nous avons exploré PyTorch pour créer un modèle capable de reconnaître les émotions faciales. Nous avons obtenu un modèle atteignant une précision de 81%, nous avons intégré ce dernier dans une application Web et une application mobile à l'aide de Flutter.

Comment Utiliser ce Projet :

    Installation des Dépendances
    Assurez-vous d'installer toutes les dépendances nécessaires en exécutant le fichier requirements.txt à la racine du projet.

    pip install -r requirements.txt

    Exploration du Code
    Explorez le code source dans les répertoires dédiés à chaque section du projet. Chaque dossier contient des commentaires pour faciliter la compréhension. Tout les fichiers python doivent être lancés à partir du répertoire où il se trouve.

    Exécution des Modèles
    Suivez les instructions spécifiques à chaque modèle pour l'entraîner ou l'utiliser. Assurez-vous de disposer des ensembles de données appropriés pour l'entraînement.


Pour l'Application Web :

    Assurer vous d'avoir une camera sur votre ordinateur, et d'avoir installer node.js avec les commandes suivantes :
        Sur Linux :
            sudo apt install nodejs
            sudo apt install npm
        Sur MacOS (avec homebrew):
            brew install node

    Il faut se trouver dans le répertoire Application/src et lancer la commande suivante :
        node server.js

    Une fois le serveur lancer sur votre terminal, ouvrez votre navigateur web.
    Copier le lien suivant et copier le sur le navigateur : 
        http://localhost:8080/

    Acceptez que le site utilise votre caméra. Et voilà !
    Vous pouvez utiliser notre site pour reconnaitre votre émotion.
    
6. Non abouti

Nous avons essayer de reproduire une architecture de réseau de neurones nommé PAtt-Lite, mais sans succès.
Nous avons aussi essayer de reproduire un GAN, mais la précision de ce dernier était de 20% donc nous avons abandonné pour se concentrer sur le projet final.

Merci d'avoir exploré notre projet ! 🚀