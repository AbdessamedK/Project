#ifndef _CORPS_H
#define _CORPS_H

#include "NBComplex.h"

/// @brief Direction du déplacement
enum Mouvement {H,B,G,D,HD,HG,BD,BG};

/// @class Corps
class Corps 
{
    public : 

        /// @brief Position du corps
        NBComplex pos;
        
        /// @brief stat
        int pvmax;
        
        /// @brief Constructeur par défaut de la class Corps
        Corps();

        /// @brief Constructeur de la class Corps
        /// @param _pv : nombre de points de vie
        /// @param _degat : les dégats du personnage
        /// @param _vitesse : la vitesse de déplacement du personnage
        /// @param _vitesseAtt : vitesse d'attaque du personnage
        Corps(int _pv, int _degat, int _vitesse, int _vitesseAtt, float _x, float _y);

        
        //~Corps() { for... delete pouvoirs[i]; }
        /// @brief Destructeur de la classe Corps
        ~Corps();

        /// @brief Affiche le nombre de PV restant d'un personnage
        /// @return int
        int getPV() const;

        /// @brief Affiche le nombre de degat d'un personnage
        /// @return int
        int getDegat() const;

        /// @brief Affiche la vitesse d'un personnage
        /// @return int
        int getVitesse() const;

        /// @brief Affiche la vitesse d'attaque d'un personnage
        /// @return int
        int getVitesseAtt() const;

        /// @brief Récupère la direction du mouvement
        /// @return Mouvement
        Mouvement getMouv() const ;

        /// @brief Permet de voir si un personnage a été touché
        /// @param p : NBComplex
        /// @return bool
        bool estTouche(const NBComplex &p) const;

        /// @brief Inflige des dégats
        /// @param attaque les dégâts subis/ les pv à enlever
        void enlevePV(const int attaque);

        /// @brief Test de régression de la class Corps pour s'assurer 
        ///        que tout marche correctement
        /// @return void
        void testRegression();

        /// @brief Permet de mettre à jour les données
        void updateMouv();

        /// @brief Permet de soigner à hauteur de pvBonus
        /// @param pvBonus les points de vie à ajouter
        void Heal(const unsigned int pvBonus);

        /// @brief Indique si un Monstre/Personnage est mort
        /// @return booleen qui indique si vivant ou non
        bool estMort() const;

        /// @brief Indique si le Monstre/Personnage se déplace
        /// @return booleen qui indique si mouvement ou non
        bool estEnDeplacement() const ;

        /// @brief Soigne 
        void healPerso();

        /// @brief Augmente les dégats
        void upAttackPerso();

        /// @brief Augmente la santé max
        void upPV();

    private :
        /// @brief Statistiques
        int pv, degat,vitesse,vitesseAtt ;
        /// @brief Deplacement
        bool Deplacement ;
        /// @brief Position
        NBComplex anciennePos ;
        /// @brief Mouvement
        Mouvement mouv ;
};

#endif