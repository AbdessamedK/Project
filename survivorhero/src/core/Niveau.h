#ifndef _NIVEAU_H
#define _NIVEAU_H

#include<iostream>
#include<SDL2/SDL.h>
#include "Monstre.h"
#include<vector>

/// @class Niveau
class Niveau
{
    public:
        /// @brief Tableau Dynamique pour stocker l'ensemble des monstres d'un niveau
        std::vector<Monstre> monstres;
        
        /// @brief Constructeur par défaut de la classe Niveau
        Niveau();

        /// @brief Destructeur de la classe Niveau
        ~Niveau();

        /// @brief Indique si un niveau est terminé
        /// @return booleen
        bool estFini() const;

        /// @brief Supprime les monstres monstres du niveau
        void supprimeMonstreMorts() ; 

        /// @brief Ajoute un monstre à un Niveau
        /// @param m Le monstre à ajouter
        void ajouteMonstre(Monstre m) ;

        /// @brief Met à jour le niveau
        /// @param p Le personnage par rapport auquel les monstres vont se déplacer automatiquement
        void update(Personnage& p);

        /// @brief Indique le nombre de monstre vivants dans un niveau
        /// @return int le nombre de monstres vivants
        int nbMonstresVivants() const ;
};

#endif