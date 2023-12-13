#include "Terrain.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>


Terrain::Terrain()
{
    largeur_texture = 0;
    hauteur_texture = 0;
}

Terrain::Terrain(int _largeur_fenetre,int _hauteur_fenetre)
{
    largeur_texture = (_largeur_fenetre / 15) + 1;
    hauteur_texture = (_hauteur_fenetre / 15) + 1;
}

Terrain::~Terrain()
{
    largeur_texture = 0;
    hauteur_texture = 0;
}

void Terrain::appliquerTexture(SDL_Texture * texture, SDL_Renderer* renderer,  SDL_Rect * src_rect, int i, int j) const
{
    if (texture == nullptr) {
        std::cerr << "Erreur : " << strerror(errno) << std::endl;
        return;
    }

    if (renderer == nullptr) {
        std::cerr << "Erreur : " << strerror(errno) << std::endl;
        return;
    }

    if (src_rect == nullptr) {
        std::cerr << "Erreur : " << strerror(errno) << std::endl;
        return;
    }
        SDL_Rect dest = {largeur_texture*j, hauteur_texture*i, largeur_texture, hauteur_texture}; // Destination de la texture
        SDL_RenderCopy (renderer, texture, src_rect,&dest);
}
