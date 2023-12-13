#ifndef _PERSONNAGE_H
#define _PERSONNAGE_H

#include "Corps.h"
#include"Pouvoir.h"
#include <vector>
#include <SDL2/SDL.h>

class Pouvoir;
class Monstre;

/// @class Personnage
class Personnage
{
    public : 
        /// @brief Tableau Dynamique vector pour stocker les différents pouvoir du perso
        std::vector<Pouvoir*> pouvoirs;

        /// @brief Constructeur par défaut de la class Personnage
        Personnage ();

        /// @brief Destructeur de la class Personnage
        ~Personnage();

        /// @brief Permet de déplacer le personnage
        /// @param c l'entrée en mode txt
        void deplacementTxt(const char c);

        /// @brief Deplacement du personnage sur la fenêtre SDL
        /// @param tab_ennemi Pour mettre en place les collisions du personnage avec les monstres
        /// @param terrain Pour mettre en place les collisions du personnage avec le terrain
        void deplacement(std::vector<Monstre>& tab_ennemi, const SDL_Rect& terrain);

        /// @brief Mutateur pour modifier le booléen peutTirer
        /// @param tire Indicateur pour savoir si le personnage peut lancer son attaque
        void setPeutTirer(const bool tire);

        /// @brief Réduction des points de vies du personnage
        /// @param attaque Les dégâts subis
        void enlevePV (const int attaque);

        /// @brief Accesseur au PV actuelle du personnage
        /// @return les points de vie du personnage
        int getPV() const ;

        /// @brief Accesseur au booleen peutTirer
        /// @return booleen
        bool getPeutTirer() const ;

        /// @brief Indique si le personnage se déplace
        /// @return booleen
        bool estEnDeplacement() const;

        /// @brief Accesseur à la position du personnage
        /// @return NBComplex
        NBComplex& getPos();

        /// @brief Permet au personnage d'éxécuter un pouvoir
        /// @param tab_pos_aura Tableau contenant les positions des aura avant le déplacement du personnage
        /// @param tab_pos_boucl Tableau contenant les positions des bouclier avant le déplacement du personnage
        /// @return void
        void executePouvoir(std::vector<Monstre> &tab_ennemi, const  NBComplex tab_pos_aura[3], const  NBComplex tab_pos_boucl[3]);

        /// @brief Permet d'ajouter un pouvoir à un personnage
        /// @param p : Pouvoir
        /// @return void
        void ajouterPouvoir(Pouvoir *p);              
        // coprs.ajouterPouvoir( new PouvoirBasique() )
        // coprs.ajouterPouvoir( new PouvoirBouclier() )

        /// @brief La boucle d'animation (déplacment & attaque) du personnage
        /// @param tab_ennemi Contient tous les monstres du niveau pour les collisions
        /// @param terrain Pour les collision avec le terrain
        void boucle(std::vector<Monstre>& tab_ennemi, const SDL_Rect& terrain);

        /// @brief Met ricoche à true
        void activeRicoche();

        /// @brief Met tir_multiple à true
        void activeTirMultiple();

        /// @brief Supprime le pouvoir passé en paramètre
        /// @param i : indice du pouvoir à supprimer
        void supprimerPouvoir(int i);

        /// @brief Le sens de déplacement du personnage
        /// @return Mouvement (enum)
        Mouvement getMouv() const ;

        /// @brief Met à jour l'affichage dans le mode txt
        /// @param tab_ennemi Les ennemis présents sur la carte
        /// @param c la touche saisie
        void update(std::vector<Monstre> &tab_ennemi, const char c);

        /// @brief Accesseur à la hitbox du personnage
        /// @return SDL_Rect qui correspond à la hitbox
        SDL_Rect getHitbox() const;

        /// @brief Permet de dessiner la barre de vie du personnage
        /// @param w Largeur de la texture de la barre de vie
        /// @param h Hauteur de la texture de la barre de vie
        /// @return SDL_Rect qui correspond à la source où l'on copie la barre de vie
        SDL_Rect drawPersoBarreVie(int w, int h) const;

        /// @brief Soigne le personnage
        void heal ();

        /// @brief Augmente les dégat
        void upAttaque();

        /// @brief Augmente les PV maximum du perso
        void upPvMax();


        /// @brief Test de régression de la class Corps pour s'assurer 
        ///        que tout marche correctement
        void testRegression () const ;

    private :
        /// @brief Rectangle représentant la hitbox du personnage
        SDL_Rect hitbox ;
        /// @brief Corps du personnage
        Corps corps;
        /// @brief Booléen qui indique si le joueur peut tirer ou non
        bool peutTirer ;
        /// @brief Booléen qui indique si les flèches du personnage doivent ricocher ou non
        bool ricoche;
        /// @brief Booléen qui indique si le joueur doit effectuer un tir multiple (tir 3 fois dans 3 directions différentes)
        bool tir_multiple;
};

#endif