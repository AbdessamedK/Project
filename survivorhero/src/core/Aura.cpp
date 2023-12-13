#include"Aura.h"
#include<iostream>
#include<assert.h>
#include<math.h>
using namespace std;


void Aura::rotation(const NBComplex & posJoueur, const NBComplex tabDiff[3], const float lambda)
{
    for(int i = 0 ; i < 3 ; i++)
    {
        posAura[i] = posJoueur + tabDiff[i];
        posAura[i] = posAura[i].rotate(posJoueur,lambda);
    }
}
Aura::Aura(const NBComplex & posJoueur)
{
    posAura[0] = posJoueur + NBComplex(0,120);
    posAura[1] = posAura[0].rotate(posJoueur,90);
    posAura[2] = posAura[1].rotate(posJoueur,90);

    largeur = 40;

    for(int i = 0 ; i < 3 ; i++){
        hitbox[i] = {(int) posAura[i].re - (largeur/2), (int) posAura[i].im - (largeur/2), largeur,largeur};
    }
}

bool Aura::estTouche(const Monstre & m) const
{
    bool estT = false;
    SDL_Rect monstre = m.getHitbox();
    for (int i = 0 ; i < 3; i++)
        if (SDL_HasIntersection(&hitbox[i], &monstre)) estT = true;
    return estT;
}

void Aura::getPos(std::vector<NBComplex> &tab) const
{
    for(int i = 0; i < 3; i++)
    {
        tab.push_back(posAura[i]);
    }
}

int Aura::getLargeur() const
{
    return largeur;
}

void Aura::getHitbox(SDL_Rect _hitbox[3]) const
{
    for(int i = 0; i < 3 ; i++)
    {
        _hitbox[i] = hitbox[i];
    }
}

void Aura::execute(const NBComplex &pos, std::vector<Monstre>& tab_ennemi,const  NBComplex tab_pos_aura[3] , const  NBComplex tab_pos_boucl[3],unsigned int ricochet , bool tir_multiple, const int vitatt, const int degat, const bool estDeplacement, bool& peutTirer)
{
    rotation(pos,tab_pos_aura,0.08);

    for(int i = 0 ; i < 3 ; i++){
        hitbox[i] = {(int) posAura[i].re - (largeur/2), (int) posAura[i].im - (largeur/2), largeur,largeur};
    }

    for(unsigned int i = 0 ; i < tab_ennemi.size(); i++)
    {
        if(estTouche(tab_ennemi[i])) tab_ennemi[i].enlevePV(2);
    }
}

void Aura::testRegression() const
{
    NBComplex centre (500,500);
    Aura temp(centre);

    // Verification des positions //
    std::vector<NBComplex> tab;
    temp.getPos(tab);

    //1ère aura
    NBComplex complextmp;
    complextmp =  tab[0];
    assert(complextmp.im == 620);
    //2 ème 
    complextmp = complextmp.rotate(centre,90);
    assert(complextmp.re == tab[1].re && complextmp.im == tab[1].im );
    //3ème Aura
    complextmp = complextmp.rotate(centre,90);
    assert(complextmp.re == tab[2].re && complextmp.im == tab[2].im );


    //Verification de la largeur //
    int large = temp.getLargeur();

    // Verification des hitbox //
    SDL_Rect rect[3];
    temp.getHitbox(rect);
    for(int i = 0 ; i < 3 ; i++){
        SDL_Rect rectmp {(int) tab[i].re - (large/2) , (int) tab[i].im - (large/2), large,large};
        assert(rect[i].x == rectmp.x);
        assert(rect[i].y == rectmp.y);
        assert(rect[i].w == rectmp.w);
        assert(rect[i].h == rectmp.h);
    }


    
}