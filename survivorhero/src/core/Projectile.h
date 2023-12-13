#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include"NBComplex.h"

/// @class Projectile
class Projectile 
{
    public : 
    /// @brief Position du projectile
    NBComplex pos; 

    /// @brief Vecteur directeur du projectile
    NBComplex vecDir; 
    
    /// @brief Nombre de ricochet que doit faire le projectile
    unsigned int ricochet;

    /// @brief Constructeur du projecticle
    Projectile() {}

    /// @brief Destructeur de projectile
    ~Projectile() {}
};

#endif