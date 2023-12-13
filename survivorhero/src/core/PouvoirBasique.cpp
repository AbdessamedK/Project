#include"PouvoirBasique.h"
#include"Monstre.h"
#include<iostream>
#include<math.h>
#include<SDL2/SDL.h>
#include<cassert>

PouvoirBasique::PouvoirBasique(int _nbmax) {}
PouvoirBasique::~PouvoirBasique() {}

bool PouvoirBasique::estTouche(const Projectile & p, const NBComplex pos2) const 
{
    return p.pos.distance (pos2) <= 40.0;
}

void PouvoirBasique::afficheBasique() const 
{
    unsigned int i=0; 
    if (ListBasique.size()>0)
    {
        for (auto actu=ListBasique.begin(); actu != ListBasique.end(); ++actu)
        {
            Projectile temp = *actu ;
            std::cout<< "Position de la particule " << i << " " ; std::cout<< temp.pos ;
            ++i ;
        } 
    }
}

void PouvoirBasique::getPos(std::vector<Projectile> &tab) const 
{
    if (ListBasique.size()>0)
    {
        for (auto actu=ListBasique.begin(); actu != ListBasique.end(); ++actu)
        {
            Projectile temp = *actu ;
            tab.push_back(temp) ;
        }
    }
}

NBComplex PouvoirBasique::EnnemiPlusProche (const NBComplex &posJoueur, std::vector<Monstre> tab_ennemi) const
{
    int taille = tab_ennemi.size() ;
    if (taille==1)
        return tab_ennemi[0].getPos() ;
    else 
    {
        NBComplex res = tab_ennemi[0].getPos() ;
        float dmin = posJoueur.distance(res) ;
        for (unsigned int i=1; i<tab_ennemi.size(); ++i)
        {
            float d = posJoueur.distance(tab_ennemi[i].getPos()) ;
            if (d<dmin) 
            {
                dmin = d ;
                res = tab_ennemi[i].getPos() ;
            }
            
        }
        return res ;
    }
}

void PouvoirBasique::update(std::vector<Monstre> &tab_ennemi, const int vitatt, const int attaque)
{
    for (auto actu=ListBasique.begin(); actu != ListBasique.end(); ++actu)
    {
        bool temp = false ;
        actu->pos = actu->pos + actu->vecDir * (vitatt/2.0);
        
        if ((actu->pos.re >=1000) || (actu->pos.re<=0) || (actu->pos.im>=800) || (actu->pos.im<=0))
        {
                temp = true ;
        }
        
        if (tab_ennemi.size()>0)
        {
            for (unsigned int i=0; i<tab_ennemi.size(); ++i)
            {
                if (estTouche(*actu,tab_ennemi[i].getPos()))
                {
                    tab_ennemi[i].enlevePV(attaque);
                    if(actu->ricochet == 1 || tab_ennemi.size() <= 1)
                    {
                        temp = true ;
                    }
                    else
                    {
                        std::vector<Monstre> tabtemp = tab_ennemi;
                        tabtemp.erase(tabtemp.begin() + i);
                        NBComplex pos_ennemi = EnnemiPlusProche(actu->pos, tabtemp);
                        NBComplex vecDirecteur = pos_ennemi - actu->pos;
                        float norme = sqrt(vecDirecteur.re*vecDirecteur.re + vecDirecteur.im*vecDirecteur.im);
                        vecDirecteur = vecDirecteur * (1/norme);
                        actu->vecDir = vecDirecteur ;
                        for(int k = 0 ; k < 5 ; k++)
                        {
                            actu->pos = actu->pos + actu->vecDir * (vitatt/2.0); // On modifie la position pour qu'à la prochaine boucle, il ne rentre pas dans estTouche.
                        }
                        actu->ricochet = actu->ricochet - 1;

                    }
                }
            }   
        }
        
        if (temp)
        {
            actu = ListBasique.erase(actu) ;
            //std::cout<<ListBasique.size()<<std::endl;
        }
    }
}

void PouvoirBasique::createProjectile(const NBComplex & posJoueur, const std::vector<Monstre> tab_ennemi, bool& peutTirer, unsigned int ricochet , bool tir_multiple)
{
    if (tab_ennemi.size()>0)
    {
        NBComplex pos_ennemi = EnnemiPlusProche(posJoueur,tab_ennemi) ;
        if (peutTirer) 
        {
            Projectile proj;
            proj.pos = posJoueur;
            NBComplex vecDirecteur = pos_ennemi - posJoueur;
            float norme = sqrt(vecDirecteur.re*vecDirecteur.re + vecDirecteur.im*vecDirecteur.im);
            vecDirecteur = vecDirecteur * (1/norme);
            proj.vecDir = vecDirecteur ;
            proj.ricochet = ricochet;
            ListBasique.push_back(proj);
            peutTirer = false ;
            if(tir_multiple) // Créer 2 autres projectiles sur les cotés
            {
                pos_ennemi = pos_ennemi.rotate(posJoueur,70);
                vecDirecteur = pos_ennemi - posJoueur;
                norme = sqrt(vecDirecteur.re*vecDirecteur.re + vecDirecteur.im*vecDirecteur.im);
                vecDirecteur = vecDirecteur * (1/norme);
                proj.vecDir = vecDirecteur ;
                proj.ricochet = ricochet;
                ListBasique.push_back(proj);



                pos_ennemi = pos_ennemi.rotate(posJoueur,-140);
                vecDirecteur = pos_ennemi - posJoueur;
                norme = sqrt(vecDirecteur.re*vecDirecteur.re + vecDirecteur.im*vecDirecteur.im);
                vecDirecteur = vecDirecteur * (1/norme);
                proj.vecDir = vecDirecteur ;
                proj.ricochet = ricochet;
                ListBasique.push_back(proj);
            }
        }
    }
}

void PouvoirBasique::execute(const NBComplex &pos, std::vector<Monstre> &tab_ennemi,const  NBComplex tab_pos_aura[3] , const  NBComplex tab_pos_boucl[3],unsigned int ricochet , bool tir_multiple, const int vitatt, const int degat, const bool estEnDeplacement, bool& peutTirer)
{
    if (!estEnDeplacement)
        createProjectile(pos,tab_ennemi,peutTirer,ricochet, tir_multiple);
    if (ListBasique.size()>0) 
        update(tab_ennemi,vitatt,degat);
}