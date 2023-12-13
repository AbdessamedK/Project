#ifndef _MONDE_H
#define _MONDE_H

#include"Niveau.h"
#include"Personnage.h"
#include"Terrain.h"

class Monde 
{
    public : 
        /// @brief Constructeur par défaut de la class Monde
        Monde();

        /// @brief Constructeur pour la version txt
        /// @param versionTxt booleen pour dissocier la version sdl et la version txt 
        Monde(const bool versionTxt);

        /// @brief Destructeur de la class Monde
        ~Monde() {}

        /// @brief Boucle d'affichage de la class
        void boucle();

        /// @brief Met à jour l'affichage txt
        /// @param c Touche enfoncé 
        void updateTxt(const char c) ;

        /// @brief Recupère le niveau actuelle
        /// @return int correspondant au niveau
        int getNiveauActu() const ;

        /// @brief Recupère le terrain actuelle
        /// @return int correspond au numéro de terrain actuel
        int getTerrainActu() const ;

        /// @brief Test de regression de la classe
        void testRegression() const ;

        /// @brief Personnage du monde
        Personnage perso ;
        /// @brief Tableau de niveaux contenant tous les niveaux du monde
        Niveau n[15] ;
        /// @brief Terrain du monde 
        Terrain terrain;
    
    private :
        /// @brief Entier pour stocké le dernier passage dans la boucle
        int lastUpdate ;
        /// @brief Entier représentant le niveau actuelle
        int niveauActu ;
        /// @brief int représentant le terrain actuelle
        int terrainActu ;
};

#endif