#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<string.h>
#include<string>
#include<vector>

/// @class Terrain
class Terrain 
{
    public :

    /// @brief Constructeur par défaut de la classe Terrain
    Terrain();

    /// @brief Constructeur avec paramètre pour la classe Terrain
    /// @param _hauteur_fenetre hauteur de la fenetre SDL
    /// @param _largeur_fenetre largueur de la fenetre SDL
    Terrain(int _largeur_fenetre,int _hauteur_fenetre) ;

    /// @brief Destructeur par défaut de la class Terrain
    ~Terrain();

    /// @brief A partir de la SDL_Texture charge la texture dans tabTerrain où c est égale tabTerrain[ligne][col]
    /// @param texture La texture à charger
    /// @param renderer le renderer
    /// @param src_rect Le rect source où l'on copie la texture
    /// @param i  L'indice de la ligne
    /// @param j L'indice de la col
    void appliquerTexture (SDL_Texture * texture, SDL_Renderer* renderer,  SDL_Rect * src_rect, int i, int j) const ;

    private :
    /// @brief Les largeurs des tile qu'on va appliqué au terrain
    int largeur_texture;

    /// @brief La hauteur des tiles qu'on va appliqué au terrain
    int hauteur_texture;
};

#endif