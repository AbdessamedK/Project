#ifndef _AURA_H
#define _AURA_H

#include"Pouvoir.h"
#include "NBComplex.h"

/// @class Aura
class Aura : public Pouvoir
{
    private :
        /// @brief int : degat provoqué par l'aura
        int degat;
        /// @brief Tableau de NBComplex represantant la position des Aura
        NBComplex posAura[3];
        /// @brief Tableau de Rectangle représentant la hitbox des Aura
        SDL_Rect hitbox[3];
        /// @brief int : Largeur d'une aura
        int largeur;

    public :
        /// @brief Constructeur de la class Aura
        /// @param posJoueur Position actuelle du joueur
        Aura(const NBComplex & posJoueur);

        /// @brief Effectue la rotation du pouvoir
        /// @param posJoueur Position actuelle du joueur
        /// @param lambda Angle de rotation
        void rotation(const NBComplex & posJoueur, const NBComplex tabDiff[3], const float lambda);

        /// @brief Indique si l'Aura touche un monstre
        /// @param nb Position du monstre
        /// @return booleen qui indique si touche ou non
        bool estTouche(const Monstre & m) const ;

        /// @brief Permet l'affichage en mode txt
        void afficheTxt() const;
        
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