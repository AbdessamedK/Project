#ifndef _POUVOIR_H
#define _POUVOIR_H

#include "NBComplex.h"
#include "Monstre.h"
#include "Projectile.h"
#include <vector>

class Monstre;

/// @class Pouvoir
class Pouvoir
{
    public : 
        /// @brief Constructeur par défaut de la classe Pouvoir
        Pouvoir () {}

        /// @brief Destructeur de la classe Pouvoir
        virtual ~Pouvoir () {}

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
        virtual void execute(const NBComplex &pos, std::vector<Monstre> &tab_ennemi,const  NBComplex tab_pos_aura[3] , const  NBComplex tab_pos_boucl[3],unsigned int ricochet , bool tir_multiple,  const int vitatt, const int degat, const bool estEnDeplacement, bool& peutTirer){} ;

        /// @brief Affiche l'attaque Basique du personnage
        virtual void afficheBasique() const {}

        /// @brief Accede à la position des projectiles
        /// @param tab Tableau Dynamique de Projectile
        virtual void getPos(std::vector<Projectile>& tab)const {}

        /// @brief Accede aux positions 
        /// @param tab Tableau Dynamique de NBComplex
        virtual void getPos(std::vector<NBComplex>& tab)const {}


        /// @brief Renvoie la largeur d'une aura
        virtual int getLargeur() const {return 0;}

        /// @brief Met dans _hitbox la hitbox des 3 auras
        /// @param _hitbox Tableau de 3 rectangles contenant la hitbox
        virtual void getHitbox(SDL_Rect _hitbox[3]) const {}
};

#endif
