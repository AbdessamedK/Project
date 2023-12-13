#include"Monde.h"
#include"Monstre.h"
#include"Aura.h"
#include"PouvoirBasique.h"
#include"Bouclier.h"
#include<iostream>
#include<cassert>
#include<SDL2/SDL.h>
using namespace std;

Monde::Monde()
{
    niveauActu = 0 ;
    terrainActu = 1 ;
    
    n[0].ajouteMonstre(Monstre(NBComplex(500,200),Basique));
    
    n[1].ajouteMonstre(Monstre(NBComplex(200,400),Basique));
    n[1].ajouteMonstre(Monstre(NBComplex(800,400),Basique));
    
    n[2].ajouteMonstre(Monstre(NBComplex(200,300),Basique)) ;
    n[2].ajouteMonstre(Monstre(NBComplex(800,300),Basique)) ;
    n[2].ajouteMonstre(Monstre(NBComplex(500,100),Fox)) ;
    
    n[3].ajouteMonstre(Monstre(NBComplex(150,200),Fox)) ;
    n[3].ajouteMonstre(Monstre(NBComplex(350,200),Fox)) ;
    n[3].ajouteMonstre(Monstre(NBComplex(650,200),Fox)) ;
    n[3].ajouteMonstre(Monstre(NBComplex(850,200),Fox)) ;

    n[4].ajouteMonstre(Monstre(NBComplex(500,300),Bleu)) ;

    n[5].ajouteMonstre(Monstre(NBComplex(300,300),Basique)) ;
    n[5].ajouteMonstre(Monstre(NBComplex(700,300),Robot)) ;

    n[6].ajouteMonstre(Monstre(NBComplex(500,300),Robot)) ;
    n[6].ajouteMonstre(Monstre(NBComplex(250,150),Fox)) ;
    n[6].ajouteMonstre(Monstre(NBComplex(750,150),Fox)) ;

    n[7].ajouteMonstre(Monstre(NBComplex(250,350),Robot)) ;
    n[7].ajouteMonstre(Monstre(NBComplex(500,350),Robot)) ;
    n[7].ajouteMonstre(Monstre(NBComplex(750,350),Robot)) ;

    n[8].ajouteMonstre(Monstre(NBComplex(250,200),Fox)) ;
    n[8].ajouteMonstre(Monstre(NBComplex(750,200),Fox)) ;
    n[8].ajouteMonstre(Monstre(NBComplex(400,350),Basique)) ;
    n[8].ajouteMonstre(Monstre(NBComplex(600,350),Robot)) ;

    n[9].ajouteMonstre(Monstre(NBComplex(500,300),Dragon)) ;

    n[10].ajouteMonstre(Monstre(NBComplex(250,400),Rouge)) ;
    n[10].ajouteMonstre(Monstre(NBComplex(650,400),Rouge)) ;

    n[11].ajouteMonstre(Monstre(NBComplex(250,300),Basique)) ;
    n[11].ajouteMonstre(Monstre(NBComplex(500,300),Spider)) ;
    n[11].ajouteMonstre(Monstre(NBComplex(750,300),Robot)) ;

    n[12].ajouteMonstre(Monstre(NBComplex(250,500),Spider)) ;
    n[12].ajouteMonstre(Monstre(NBComplex(750,200),Spider)) ;

    n[13].ajouteMonstre(Monstre(NBComplex(250,400),Basique)) ;
    n[13].ajouteMonstre(Monstre(NBComplex(750,400),Rouge)) ;
    n[13].ajouteMonstre(Monstre(NBComplex(500,400),Spider)) ;
    n[13].ajouteMonstre(Monstre(NBComplex(250,200),Robot)) ;
    n[13].ajouteMonstre(Monstre(NBComplex(600,200),Fox)) ;

    n[14].ajouteMonstre(Monstre(NBComplex(500,300),Serpent)) ;

    terrain = Terrain(1000,800);
    lastUpdate = 0 ;
}

Monde::Monde(const bool versionTxt)
{
    niveauActu = 0 ;
    lastUpdate = 0 ;
    perso.getPos() = NBComplex(50,15) ;
    n[0].ajouteMonstre(Monstre(NBComplex(20,5),Basique)) ;
}

void Monde::boucle()
{
    SDL_Rect r, porte ;
    if (SDL_GetTicks()-lastUpdate > 10) 
    {
        if (niveauActu<=4)
        {
            r = {0,155,1000,645} ;
            porte = {0,170,100,2} ;
        }
            
        else if (niveauActu<=9)
            r = {75,75,850,655} ;
        else 
        {
            r = {65,110,875,575} ;
            porte = {480,100,50,20} ;
        }
            
        SDL_Rect hitbox = perso.getHitbox() ;       
        
        perso.boucle(n[niveauActu].monstres,r) ;
        n[niveauActu].update(perso) ;
        lastUpdate = SDL_GetTicks() ;

        if (n[niveauActu].estFini() && (SDL_HasIntersection(&porte,&hitbox)|| terrainActu==2 ) )
        {
            if (niveauActu==4 || niveauActu==9)
                terrainActu++ ;
            perso.getPos().re = 500 ;
            perso.getPos().im = 600 ;
            if (niveauActu<14)
                niveauActu++ ;
            int nb_pouvoir = perso.pouvoirs.size();
            if(nb_pouvoir == 3)
            {
                perso.supprimerPouvoir(2);
                perso.supprimerPouvoir(1);
            }else if(nb_pouvoir == 2)
            {
                perso.supprimerPouvoir(1);
            }

            if(nb_pouvoir == 3)
            {
                perso.ajouterPouvoir(new Aura(perso.getPos()));
                perso.ajouterPouvoir(new Aura(perso.getPos()));
            }
            else if(nb_pouvoir == 2)
            {
                perso.ajouterPouvoir(new Aura(perso.getPos()));
            }
        }
    }
}

void Monde::updateTxt(const char c)
{
    if (SDL_GetTicks()-lastUpdate > 300)
    {
        perso.setPeutTirer(true) ;
        lastUpdate = SDL_GetTicks() ;
    }
        
    //perso.update(n[niveauActu].monstres,c);
    //n[niveauActu].update(perso);
}

int Monde::getNiveauActu() const 
{
    return niveauActu ;
}

int Monde::getTerrainActu() const 
{
    return terrainActu ;
}

void Monde::testRegression() const 
{
    Monde w ;
    
    NBComplex c(20,30) ;
    assert(c.re==20);
    assert(c.im==30);

    /*Monstre m(c) ;
    assert(m.corps.pos.re==20);
    assert(m.corps.pos.im==30);
    w.n.ajouteMonstre(m);
    assert(w.n.nbMonstresVivants()==1);
    assert(w.n.estFini()==false);
    m = Monstre(NBComplex(50,60)) ;
    w.n.ajouteMonstre(m) ;
    assert(w.n.nbMonstresVivants()==2);

    assert(w.perso.corps.pos.re==0);
    assert(w.perso.corps.pos.im==0);
    assert(w.perso.pouvoirs.size()==1);

    m = Monstre(NBComplex(4,4));
    w.n.ajouteMonstre(m);
    assert(w.n.nbMonstresVivants()==3);

    assert(w.n.monstres[2].corps.pos.re==4);
    assert(w.n.monstres[2].corps.pos.im==4);

    w.perso.executePouvoir(false,w.n.monstres);
    w.perso.pouvoirs[0]->afficheBasique();
    std::cout<< "PV initial :" << w.n.monstres[2].corps.getPV() <<std::endl ;
    w.perso.executePouvoir(true,w.n.monstres);
    w.perso.pouvoirs[0]->afficheBasique();
    std::cout<< "PV apres une particule :" << w.n.monstres[2].corps.getPV() <<std::endl ;
    std::cout<< "NB monstre sans morts : " << w.n.monstres.size() <<std::endl ;
    w.perso.executePouvoir(false,w.n.monstres);
    w.perso.pouvoirs[0]->afficheBasique();
    w.perso.executePouvoir(true,w.n.monstres);
    w.perso.pouvoirs[0]->afficheBasique();
    std::cout<< "PV apres 2 particule :" << w.n.monstres[2].corps.getPV() <<std::endl ;
    w.n.supprimeMonstreMorts();
    assert(w.n.nbMonstresVivants()==2);*/

}