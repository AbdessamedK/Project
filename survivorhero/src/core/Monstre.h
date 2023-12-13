#ifndef _MONSTRE_H
#define _MONSTRE_H

#include"Corps.h"
#include<SDL2/SDL.h>
#include"Personnage.h"
#include"Projectile.h"

class Personnage;

/// @brief Différents types des monstres
enum Type {Basique,Rouge,Fox,Robot,Spider,Bleu,Dragon,Serpent};

/// @class Monstre
class Monstre
{
    public :

    /// @brief Constructeur par défaut de la class Monstre
    Monstre ();

    /// @brief Constructeur parametré de la class Monstre
    /// @param pos Initialise la position du Personnage
    /// @param _type Initialise le type du Monstre
    Monstre(const NBComplex& pos, Type _type);

    /// @brief Destructeur de la class Monstre
    ~Monstre() {}

    /// @brief Recupère la position du monstre
    /// @return NBComplex correspond à la position
    NBComplex& getPos() ;

    /// @brief Enleve des points de vie au personnage/hero
    /// @param attaque dégâts infligés
    void enlevePV(const int attaque) ;

    /// @brief Permet le déplacement du monstre en direction du héros
    /// @param pos : NBComplex (position du personnage jouable)
    void deplacementAuto(Personnage &p);

    /// @brief Le monstre lance une attaque automatiquement en direction de l'héro
    /// @param dir : NBComplex (direction du héro)
    bool BonneDistance(const SDL_Rect& hitbox, const SDL_Rect& hitboxperso) ;

    /// @brief L'attaque du monstre de type Robot
    /// @param posPerso Position vers laquelle l'attaque est éxécuté
    void attaqueRobot(const NBComplex& posPerso) ;

    /// @brief Indique si un monstre est mort ou non
    /// @return booleen
    bool estMort() const ;

    /// @brief Indique si le monstre peut attaquer le personnage
    /// @return booleen
    bool getPeutAttaquer() const ;

    /// @brief Indique si les monstres peuvent enlever des PV au personnages
    /// @return booleen
    bool getPeutEnlevePv() const ;

    /// @brief booleen qui permet d'afficher le missile du robot
    /// @return booleen
    bool getAffProj() const ;

    /// @brief Degat infligé par le monstre
    /// @return int
    int getAttaque() const ;

    /// @brief Recupère le projectile à afficher
    /// @return le projectile
    Projectile getProj() const ;

    /// @brief Met à jour le booleen qui indique si le monstre peut enlever des points de vies au personnage
    /// @param peutenleverpv met à jour le booleen
    void setPeutEnlevePv(const bool peutenleverpv) ;

    /// @brief Met à jour le booleen qui indique si le monstre peut attaque le personnage
    /// @param peutattaquer met à jour le booleen
    void setPeutAttaquer(const bool peutattaquer) ;

    /// @brief Recupère la hitbox du Monstre
    /// @return SDL_Rect de la hitbox
    SDL_Rect getHitbox() const ;

    /// @brief Récupère le type du Monstre
    /// @return Type
    Type getType() const ;

    /// @brief Permet d'obtenir la destination de l'affichage de la barre de vie du Monstre
    /// @return SDL_Rect de la barre de vie
    SDL_Rect getMonstreBarreVie () const;

    /// @brief Dessine la barre de vie du personnage
    /// @param w Largeur de la texture de la barre de vie
    /// @param h hauteur de la texture de la barre de vie
    /// @return SDL_Rect destination
    SDL_Rect drawMonstreBarreVie(float _pvmax, int w, int h) const;

    private : 
    /// @brief Hauteur et largeur du monstre
    int hauteur, largeur ;

    /// @brief Position, Nombre de coup du monstre, nombre de soins du monstre
    int lastPoison, nbCoup, nbHeal ;

    /// @brief la hitbox du monstre
    SDL_Rect hitbox ;

    /// @brief la Class Corps du Monstre
    Corps corps;

    /// @brief Le type du monstre
    Type type;

    /// @brief Indique si le projectile est à afficher
    bool affProj;

    /// @brief Indique si le monstre peut attaquer le personnage
    bool peutAttaquer;

    /// @brief Indique si le monstre peut enlever des points de vie au personnage
    bool peutEnlevePv;

    /// @brief Indique si le monstre à la possibilité de se soigner
    bool seHeal;

    /// @brief Le projectile du monstre
    Projectile proj ;
};

#endif