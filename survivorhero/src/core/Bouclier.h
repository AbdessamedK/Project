#ifndef _BOUCLIER_H
#define _BOUCLIER_H

#include "NBComplex.h"
#include "Pouvoir.h"

/// @class Bouclier
class Bouclier : public Pouvoir
{
    private :

        /// @brief Tableau de NBComplex représentant les positions des boucliers
        NBComplex posBouclier[3];
        /// @brief Tableau de Rectangle représentant la hitbox des boucliers
        SDL_Rect hitbox[3];
        /// @brief int : Largeur d'un bouclier
        int largeur;

    public :

        /// @brief Constructeur de la class Bouclier
        /// @param posJoueur Position actuelle du joueur
        Bouclier(const NBComplex & posJoueur);

        /// @brief Rotation du bouclier autour du personnage
        /// @param posJoueur Position actuelle du joueur
        /// @param tabDiff Tableau contenant l'écart du perso et des boucliers avant le déplacement du perso
        /// @param lambda Angle de rotation
        void rotation(const NBComplex & posJoueur, const NBComplex tabDiff[3], const float lambda);

        /// @brief Indique si l'Aura touche un monstre
        /// @param m Monstre 
        /// @return booleen qui indique si touche ou non
        bool estTouche(const Monstre & m) const ;


        /// @brief Exécute une attaque du personnage
        /// @param pos Position de l'attaque
        /// @param tab_ennemi Contient tous les monstres du niveau
        /// @param tab_pos_aura Tableau contenant les positions des aura avant le déplacement du personnage
        /// @param tab_pos_boucl Tableau contenant les positions des bouclier avant le déplacement du personnage
        /// @param ricochet Nombre de ricochet
        /// @param vitatt Vitesse de l'attaque
        /// @param degat Degat de l'attaque
        /// @param estEnDeplacement Indique si le personnage se déplace
        /// @param peutTirer Indique si le personnage peut tirer
        void execute(const NBComplex &pos, std::vector<Monstre>& tab_ennemi, const  NBComplex tab_pos_aura[3] , const  NBComplex tab_pos_boucl[3],unsigned int ricochet , bool tir_multiple, const int vitatt, const int degat, const bool estDeplacement, bool& PeutTirer) override;

        /// @brief Stocke les valeurs du centre de chaque aura
        /// @param tab Contient les positions
        void getPos(std::vector<NBComplex> &tab)const override;


        /// @brief Renvoie la largeur d'une aura
        int getLargeur() const override;

        /// @brief Met dans _hitbox la hitbox des 3 auras
        /// @param _hitbox Tableau de 3 rectangles contenant la hitbox
        void getHitbox(SDL_Rect _hitbox[3]) const override;

        /// @brief Test de régression
        void testRegression() const;


};

#endif