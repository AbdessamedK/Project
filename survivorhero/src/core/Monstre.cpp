#include "Corps.h"
#include "Monstre.h"
using namespace std ;

Monstre::Monstre ()
{
    largeur = 0 ; hauteur = 0 ;
    type = Basique ;
    peutAttaquer = false ;
    peutEnlevePv = false ;
    affProj = false ;
    lastPoison = 0 ;
}

Monstre::Monstre(const NBComplex& pos, Type _type)
{
    switch (_type)
    {
        case Basique : 
        {
            corps = Corps(50*10,5,25,1,pos.re,pos.im) ;
            hauteur = 130 ; largeur = 120 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Rouge :
        {
            corps = Corps(70*10,5,17,1,pos.re,pos.im) ;
            hauteur = 130 ; largeur = 120 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Fox :
        {
            corps = Corps(20*10,1,50,1,pos.re,pos.im) ;
            hauteur = 100 ; largeur = 100 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Robot :
        {
            corps = Corps(30*10,5,10,1,pos.re,pos.im) ;
            hauteur = 130 ; largeur = 120 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Spider :
        {
            corps = Corps(40*10,5,30,1,pos.re,pos.im) ;
            hauteur = 100 ; largeur = 100 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Bleu :
        {
            corps = Corps(300*5,100,20,1,pos.re,pos.im) ;
            hauteur = 150 ; largeur = 140 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Dragon :
        {
            corps = Corps(300*5,10,30,1,pos.re,pos.im) ;
            hauteur = 150 ; largeur = 140 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
        case Serpent :
        {
            corps = Corps(300*5,10,30,1,pos.re,pos.im) ;
            hauteur = 150 ; largeur = 140 ;
            hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
            break ;
        }
    }
    type = _type ;
    peutEnlevePv = false ;
    peutAttaquer = false ;
    affProj = false ;
    seHeal = false ;
    lastPoison = 0 ;
    nbCoup = 0 ;
    nbHeal = 0 ;
}

NBComplex& Monstre::getPos()
{
    return corps.pos ;
}

void Monstre::enlevePV(const int attaque)
{
    corps.enlevePV(attaque) ;
}

SDL_Rect Monstre::getHitbox() const 
{
    return hitbox ;
}

Type Monstre::getType() const 
{
    return type ;
}

bool Monstre::estMort() const 
{
    return corps.estMort() ;
}

bool Monstre::getPeutEnlevePv() const 
{
    return peutEnlevePv ;
}

bool Monstre::getAffProj() const 
{
    return affProj ;
}

Projectile Monstre::getProj() const 
{
    return proj ;
}

int Monstre::getAttaque() const 
{
    return nbCoup ;
}

bool Monstre::getPeutAttaquer() const 
{
    return peutAttaquer ;
}

void Monstre::setPeutEnlevePv(const bool peutenleverpv)
{
    peutEnlevePv = peutenleverpv ;
}

void Monstre::setPeutAttaquer(const bool peutattaquer)
{
    peutAttaquer = peutattaquer ;
}

void Monstre::deplacementAuto(Personnage& p)
{
    SDL_Rect hit = p.getHitbox() ;
    switch (type)
    {
        case Fox :
        case Rouge : 
        case Basique : 
        {
            if (peutAttaquer) 
            {
                if (BonneDistance(hitbox,hit) && peutEnlevePv)
                    p.enlevePV(corps.getDegat()) ;
            }
            else
            {
                NBComplex pos(p.getPos()) ;
                NBComplex u = NBComplex((pos.re - corps.pos.re),(pos.im - corps.pos.im));   // vecteur correspondant au Monstre vers le héro
                float norme = sqrt(u.re*u.re + u.im*u.im);   // norme du vecteur u
                u = NBComplex((u.re / norme), (u.im / norme));

                corps.pos.re += u.re * (corps.getVitesse()/40.0);  // la vitesse de déplacement dépend des stats du Monstre
                corps.pos.im += u.im * (corps.getVitesse()/40.0);
            
                int hauteur = 130, largeur = 120 ;
                hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;

                peutAttaquer = BonneDistance(hitbox,hit) ;
            }
            break ;
        }
        case Robot :
        {
            if (peutEnlevePv) // cree la rocket
            {
                proj.pos = NBComplex(corps.pos) ;
                NBComplex vecDir = p.getPos() - corps.pos ;
                float norme = sqrt(vecDir.re*vecDir.re + vecDir.im*vecDir.im) ;
                vecDir = vecDir * (1/norme) ;
                proj.vecDir = vecDir ;
                peutEnlevePv = false ;
            }
            else if (proj.pos.re == 0 && proj.pos.im == 0) // le robot se deplace jusqu'a etre a la bonne distance du joueur pour lancer sa rocket
            {
                NBComplex pos(p.getPos()) ;
                NBComplex u = NBComplex((pos.re - corps.pos.re),(pos.im - corps.pos.im));   // vecteur correspondant au Monstre vers le héro
                float norme = sqrt(u.re*u.re + u.im*u.im);   // norme du vecteur u
                u = NBComplex((u.re / norme), (u.im / norme));

                corps.pos.re += u.re * (corps.getVitesse()/40.0);  // la vitesse de déplacement dépend des stats du Monstre
                corps.pos.im += u.im * (corps.getVitesse()/40.0);
            
                hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ; 
                attaqueRobot(p.getPos()) ;
            }
            
            if (proj.pos.re != 0 && proj.pos.im != 0) // met a jour la rocket
            {
                affProj = true ;
                proj.pos = proj.pos + proj.vecDir * 4 ;
                if ((proj.pos.distance(p.getPos()) <= 40.0) || (proj.pos.re>=1000) || (proj.pos.re<=0) || (proj.pos.im>=800) || (proj.pos.im<=0)) // si la rocket touche le joueur ou deborde de la fenetre
                {
                    if (proj.pos.distance(p.getPos()) <= 40.0)
                        p.enlevePV(corps.getDegat()) ;
                    affProj = false ;
                    proj.pos = NBComplex(0,0) ; proj.vecDir = NBComplex(0,0) ;
                    peutEnlevePv = false ;
                    peutAttaquer = true ;
                }
            }
            break ;
        }
        case Spider : 
        {
            if ((nbCoup > 0) && (SDL_GetTicks()-lastPoison >= 700))
            {
                p.enlevePV(corps.getDegat()) ;
                lastPoison = SDL_GetTicks() ;
                nbCoup = (nbCoup+1)%4 ;
            }
            
            if (peutAttaquer)
            {
                if (peutEnlevePv && BonneDistance(hitbox,hit) && (nbCoup==0))
                {
                    p.enlevePV(corps.getDegat()) ;
                    nbCoup ++ ;
                }
            }
            else if (nbCoup == 0)
            {
                NBComplex pos(p.getPos()) ;
                NBComplex u = NBComplex((pos.re - corps.pos.re),(pos.im - corps.pos.im));   // vecteur correspondant au Monstre vers le héro
                float norme = sqrt(u.re*u.re + u.im*u.im);   // norme du vecteur u
                u = NBComplex((u.re / norme), (u.im / norme));

                corps.pos.re += u.re * (corps.getVitesse()/40.0);  // la vitesse de déplacement dépend des stats du Monstre
                corps.pos.im += u.im * (corps.getVitesse()/40.0);
            
                int hauteur = 100, largeur = 100 ;
                hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;

                peutAttaquer = BonneDistance(hitbox,hit) ;
            }
            break ;
        }
        case Dragon :
        case Bleu :
        {
            if (peutAttaquer)
            {
                if (peutEnlevePv && BonneDistance(hitbox,hit))
                {
                    if (nbCoup==0) // attaque 1
                        p.enlevePV(corps.getDegat()) ;
                    else  // attaque 2
                        p.enlevePV(corps.getDegat()) ;
                }
            }
            else 
            {
                nbCoup = (rand()%2) ;
                NBComplex pos(p.getPos()) ;
                NBComplex u = NBComplex((pos.re - corps.pos.re),(pos.im - corps.pos.im));   // vecteur correspondant au Monstre vers le héro
                float norme = sqrt(u.re*u.re + u.im*u.im);   // norme du vecteur u
                u = NBComplex((u.re / norme), (u.im / norme));

                corps.pos.re += u.re * (corps.getVitesse()/40.0);  // la vitesse de déplacement dépend des stats du Monstre
                corps.pos.im += u.im * (corps.getVitesse()/40.0);
            
                int hauteur = 150, largeur = 140 ;
                hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;

                peutAttaquer = BonneDistance(hitbox,hit) ;
            }
            break ;
        }
        case Serpent :
        {
            if (peutAttaquer)
            {
                if (nbCoup==0)
                {
                    if (peutEnlevePv && BonneDistance(hitbox,hit))
                        p.enlevePV(corps.getDegat()) ;
                }
                else 
                {
                    if (peutEnlevePv)
                    {
                        corps.Heal(30) ;
                        lastPoison = SDL_GetTicks() ;
                    }
                        
                }
            }
            else
            {
                NBComplex pos(p.getPos()) ;
                NBComplex u = NBComplex((pos.re - corps.pos.re),(pos.im - corps.pos.im));   // vecteur correspondant au Monstre vers le héro
                float norme = sqrt(u.re*u.re + u.im*u.im);   // norme du vecteur u
                u = NBComplex((u.re / norme), (u.im / norme));

                corps.pos.re += u.re * (corps.getVitesse()/40.0);  // la vitesse de déplacement dépend des stats du Monstre
                corps.pos.im += u.im * (corps.getVitesse()/40.0);
            
                int hauteur = 150, largeur = 140 ;
                hitbox = {(int) corps.pos.re-(largeur/2), (int) corps.pos.im-(hauteur/2),largeur,hauteur} ;
                
                if (BonneDistance(hitbox,hit))
                {
                    peutAttaquer = true ;
                    nbCoup = 0 ;
                }
                else if (corps.getPV()<100*5 && SDL_GetTicks()-lastPoison>5000 && nbHeal<4)
                {
                    peutAttaquer = true ;
                    nbCoup = 1 ;
                    nbHeal++ ;
                }
            }
            break ;
        }
    }
    corps.updateMouv() ;
}

bool Monstre::BonneDistance(const SDL_Rect& hitbox, const SDL_Rect& hitboxperso) 
{
    SDL_Rect hit = {hitbox.x-20,hitbox.y-20,hitbox.w+40,hitbox.h+40} ;
    return SDL_HasIntersection(&hit,&hitboxperso) ;
}

void Monstre::attaqueRobot(const NBComplex& posPerso)
{
    peutAttaquer = corps.pos.distance(posPerso) <= 300 ;
}

SDL_Rect Monstre::getMonstreBarreVie() const
{
    return SDL_Rect {(int) corps.pos.re - 70, (int) corps.pos.im - 80 , 150, 5};
}

SDL_Rect Monstre::drawMonstreBarreVie(float _pvmax, int w, int h) const 
{
    int bw = (int)(((float)corps.getPV() / _pvmax) * (float)w); // Calcule de la barre de vie en fonction de la santé actuelle du monstre
    return SDL_Rect {0,0, bw, h};
}