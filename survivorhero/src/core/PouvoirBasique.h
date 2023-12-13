#ifndef _POUVOIRBASIQUE_H
#define _POUVOIRBASIQUE_H

#include"Pouvoir.h"
#include<list>
#include<vector>

/// @class PouvoirBasique
class PouvoirBasique : public Pouvoir
{
    private: 
    
    /// @brief Liste contenant tous les projectiles
    std::list<Projectile> ListBasique ;

    /// @brief Indique si le projectile à toucher un ennemi
    /// @param p le projectile pour lequel on fait la vérification
    /// @param pos2 la position de l'ennemi
    /// @return booleen
    bool estTouche(const Projectile & p, const NBComplex pos2) const;
    
    public:
    
    /// @brief Constructeur de la classe PouvoirBasique
    /// @param _nbmax 
    PouvoirBasique(int _nbmax=0);

    /// @brief Destructeur de la classe PouvoirBasique
    ~PouvoirBasique() override;

    /// @brief Affichage du pouvoir
    void afficheBasique() const override;

    /// @brief Accède à la position des projectiles
    /// @param tab Tableau Dynamique de Projectiles
    void getPos(std::vector<Projectile> &tab)const override;

    /// @brief Attaque l'ennemi le plus proche du personnage
    /// @param posJoueur Position actuelle du joueur
    /// @param tab_ennemi Tableau Dynamique contenant les ennemis
    /// @return NBComplex
    NBComplex EnnemiPlusProche(const NBComplex &posJoueur, std::vector<Monstre> tab_ennemi) const;

    /// @brief Initialisation du projectile lors de l'attaque
    /// @param posJoueur Position actuelle du joueur
    /// @param tab_ennemi Tableau Dynamique contenant les ennemis
    /// @param peutTirer Booleen qui indique si le personnage peut attaquer
    void createProjectile(const NBComplex & posJoueur, const std::vector<Monstre> tab_ennemi, bool& peutTirer,unsigned int ricochet, bool tir_multiple);

    /// @brief Met à jour les différentes attaques
    /// @param tab_ennemi Tableau Dynamique de monstres
    /// @param vitatt Vitesse d'attaque du personnage
    /// @param degat Dégât du personnage
    void update(std::vector<Monstre> &tab_ennemi, const int vitatt, const int degat);

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
    void execute(const NBComplex &pos, std::vector<Monstre>& tab_ennemi ,const  NBComplex tab_pos_aura[3] , const  NBComplex tab_pos_boucl[3],unsigned int ricochet , bool tir_multiple, const int vitatt, const int degat, const bool estDeplacement, bool& PeutTirer) override;
};

#endif