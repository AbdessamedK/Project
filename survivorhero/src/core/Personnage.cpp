#include "Personnage.h"
#include"PouvoirBasique.h"
#include"Bouclier.h"
#include"Aura.h"
#include <SDL2/SDL.h>
#include <math.h>
#include <cassert>
#include<iostream>
using namespace std;

Personnage::Personnage()
{
    corps = Corps(100,10*10,30,23,500,700);
    pouvoirs.push_back(new PouvoirBasique(10));
    //pouvoirs.push_back(new Aura(corps.pos)) ;
    //pouvoirs.push_back(new Bouclier(corps.pos)) ;
    peutTirer = false ;
    ricoche = false;
    tir_multiple = false;
    int hauteur = 90, largeur = 75 ;
    hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
    corps.pvmax = 100 ;
}

Personnage::~Personnage()
{
    hitbox = {0,0,0,0} ;
    for (unsigned int i=0; i<pouvoirs.size(); ++i)
    {
        delete pouvoirs[i] ;
        pouvoirs[i] = nullptr ;
    }
}

NBComplex& Personnage::getPos()
{
    return corps.pos ;
}

Mouvement Personnage::getMouv() const 
{
    return corps.getMouv() ;
}

void Collision (vector<Monstre>& tab_ennemi, const float anciennePos, float& pos, const SDL_Rect& _hitbox, const SDL_Rect& _terrain)
{
    // collision avec le terrain
    if ( (_hitbox.x <= _terrain.x) || (_hitbox.y <= _terrain.y) || (_hitbox.x+_hitbox.w >= _terrain.x+_terrain.w) || (_hitbox.y+_hitbox.h >= _terrain.y+_terrain.h) )
    {
        pos = anciennePos ;
        return ;
    }

    // collision avec les monstres
    vector<SDL_Rect> tabHitbox;
    for (unsigned int i=0; i<tab_ennemi.size(); ++i)
    {
        tabHitbox.push_back(tab_ennemi[i].getHitbox()) ;
        if (SDL_HasIntersection(&_hitbox,&tabHitbox[i]))
            pos = anciennePos ;
    }
}

void Personnage::deplacement (vector<Monstre>& tab_ennemi, const SDL_Rect& terrain)
{
    int hauteur = 90, largeur = 75 ;    
    NBComplex anciennePos = corps.pos ;
    const Uint8 *state = SDL_GetKeyboardState(NULL);    // Tableau de l'état actuel des touches du clavier.

    if (state[SDL_SCANCODE_W]){ // si la touche z est enfoncé => déplacement haut
        corps.pos.im = corps.pos.im-corps.getVitesse()/20.0 ;
        
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
        Collision(tab_ennemi,anciennePos.im,corps.pos.im,hitbox,terrain) ;
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
    }

    if (state[SDL_SCANCODE_D]){ // si la touche d est enfoncé => déplacement à droite
        corps.pos.re = corps.pos.re+corps.getVitesse()/20.0 ;
        
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
        Collision(tab_ennemi,anciennePos.re,corps.pos.re,hitbox,terrain) ;
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
    }

    if (state[SDL_SCANCODE_S]){ // si la touche s est enfoncé => déplacement bas
        corps.pos.im = corps.pos.im+corps.getVitesse()/20.0 ;
        
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
        Collision(tab_ennemi,anciennePos.im,corps.pos.im,hitbox,terrain) ;
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
    }

    if (state[SDL_SCANCODE_A]){ // si la touche q est enfoncé => déplacement gauche
        corps.pos.re = corps.pos.re-corps.getVitesse()/20.0 ;

        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
        Collision(tab_ennemi,anciennePos.re,corps.pos.re,hitbox,terrain) ;
        hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
    }
} 

void Personnage::setPeutTirer(const bool tire) {peutTirer = tire ;}

bool Personnage::getPeutTirer() const {return peutTirer;}

bool Personnage::estEnDeplacement() const 
{
    return corps.estEnDeplacement() ;
}

void Personnage::deplacementTxt(const char c)
{
    if (c=='o') 
    {
        corps.pos.im = std::max(corps.pos.im-corps.getVitesse(),float (1)) ;
    } 
    if (c=='l')
    {
        corps.pos.im = std::min(corps.pos.im+corps.getVitesse(),float (28)) ;
    }
    if (c=='k')
    {
        corps.pos.re = std::max(corps.pos.re-corps.getVitesse(), float (1)) ;
    }
    if (c=='m')
    {
        corps.pos.re = std::min(corps.pos.re+corps.getVitesse(), float (98)) ;
    }
}

void Personnage::ajouterPouvoir(Pouvoir * p)
{
    pouvoirs.push_back(p) ;
}

void Personnage::supprimerPouvoir(int i)
{
    pouvoirs.erase(pouvoirs.begin() + i);
}

void Personnage::enlevePV(const int attaque)
{
    corps.enlevePV(attaque) ;
}

int Personnage::getPV() const 
{
    return corps.getPV() ;
}

void Personnage::activeRicoche()
{
    ricoche = true;
}

void Personnage::activeTirMultiple()
{
    tir_multiple = true;
}

void Personnage::heal()
{
    corps.healPerso();
}

void Personnage::upAttaque()
{
    corps.upAttackPerso();
}

void Personnage::upPvMax()
{
    corps.upPV();
}

void Personnage::executePouvoir(std::vector<Monstre> &tab_ennemi, const  NBComplex tab_pos_aura[3], const  NBComplex tab_pos_boucl[3]) 
{
    int nb_ricochet;
    for(unsigned int i=0; i<pouvoirs.size(); ++i )
        {
            if(ricoche)
            {
                nb_ricochet = 2;
            } 
            else 
            {
                nb_ricochet = 1;
            }
            pouvoirs[i]->execute(corps.pos,tab_ennemi,tab_pos_aura, tab_pos_boucl, nb_ricochet, tir_multiple,corps.getVitesseAtt(), corps.getDegat(), corps.estEnDeplacement(), peutTirer);
        }
} 

SDL_Rect Personnage::getHitbox() const 
{
    return hitbox ;
}

void Personnage::boucle(std::vector<Monstre>& tab_ennemi, const SDL_Rect& terrain)
{
    NBComplex tabpos_avant_mouv_aura[3];
    vector<NBComplex> posAura;
    if (pouvoirs.size()>1)
    {
        pouvoirs[1]->getPos(posAura);
        for(int i = 0 ; i < 3 ; i++)
        {
            tabpos_avant_mouv_aura[i] = posAura[i] - corps.pos;
        }
    }

    NBComplex tabpos_avant_mouv_boucl[3];
    vector<NBComplex> posBoucl;

    if (pouvoirs.size()>2)
    {
        pouvoirs[2]->getPos(posBoucl);
        for(int i = 0 ; i < 3 ; i++)
        {
            tabpos_avant_mouv_boucl[i] = posBoucl[i] - corps.pos;
        }
    }

    deplacement(tab_ennemi,terrain);
    corps.updateMouv() ;
    executePouvoir(tab_ennemi,tabpos_avant_mouv_aura, tabpos_avant_mouv_boucl);
}

void Personnage::update(std::vector<Monstre>& tab_ennemi, const char c)
{
    NBComplex tabpos_avant_mouv_aura[3];
    vector<NBComplex> posAura;
    pouvoirs[1]->getPos(posAura);
    for(int i = 0 ; i < 3 ; i++)
    {
        tabpos_avant_mouv_aura[i] = corps.pos - posAura[i];
    }
    NBComplex tabpos_avant_mouv_boucl[3];
    vector<NBComplex> posBoucl;


    pouvoirs[2]->getPos(posBoucl);
    for(int i = 0 ; i < 3 ; i++)
    {
        tabpos_avant_mouv_boucl[i] = corps.pos - posBoucl[i];
    }
    deplacementTxt(c);
    executePouvoir(tab_ennemi,tabpos_avant_mouv_aura, tabpos_avant_mouv_boucl);
}

SDL_Rect Personnage::drawPersoBarreVie(int w, int h) const
{
    int bw = (int)(((float)corps.getPV() / (float)corps.pvmax) * (float)w); // Calcule de la barre de vie en fonction de la santé actuelle du monstre
    return SDL_Rect {0,0, bw, h};
}


void Personnage::testRegression () const 
{   
    Personnage p;
    // Test du constructeur par défaut
    std::cout<<"Test du constructeur par défaut (pv:"<<p.corps.getPV()<<
                ",degat:"<<p.corps.getDegat()<<",vitesse:"<<p.corps.getVitesse()<<
                ",vitesseAtt:"<<p.corps.getVitesseAtt()<<",pos("<<p.corps.pos.re<<","<<p.corps.pos.im<<"))"<<std::endl;
    assert(p.corps.getDegat() == 80);
    assert(p.corps.getPV() == 500);
    assert(p.corps.getVitesse() == 3);
    assert(p.corps.getVitesseAtt() == 1);
    assert(p.corps.pos.re == 0);
    assert(p.corps.pos.im == 0);
    std::cout << "Test terminé (OK!)" << std::endl;

    std::cout << "Position initiale du personnage : ("<< p.corps.pos.re<<","<<p.corps.pos.im<<")" << std::endl;
} 