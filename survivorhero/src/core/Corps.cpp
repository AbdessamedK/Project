#include "Corps.h"
#include "math.h"
#include <cassert>
#include <iostream>

Corps::Corps()
{
    // STATS
    pv = 0;
    degat = 0;
    vitesse = 0;
    vitesseAtt = 0;
    // POSITION
    Deplacement = false ;
    pos = NBComplex(0,0);
    anciennePos = pos ;
    mouv = H ;
}

Corps::Corps(int _pv, int _degat, int _vitesse, int _vitesseAtt, float _x, float _y)
{
    pv = _pv;
    degat = _degat;
    vitesse = _vitesse;
    vitesseAtt = _vitesseAtt;
    Deplacement = false ;
    pos = NBComplex(_x,_y);
    anciennePos = pos ;
    mouv = H ;
}

Corps::~Corps()
{
    pv = 0;
    degat = 0;
    vitesse = 0;
    vitesseAtt = 0;
    pos = NBComplex(0,0);
    anciennePos = pos ;
}

int Corps::getPV() const {return pv;}

int Corps::getDegat() const {return degat;}

int Corps::getVitesse() const {return vitesse;}

int Corps::getVitesseAtt() const {return vitesseAtt;}

Mouvement Corps::getMouv() const {return mouv;}

void Corps::enlevePV(const int attaque)
{
    pv -= attaque ;
}

bool Corps::estTouche(const NBComplex &p) const
{
    return (pos.distance(p) < 5.0);
}

void Corps::updateMouv()
{
    if (pos.re==anciennePos.re && pos.im==anciennePos.im)
        Deplacement = false ;
    else 
        Deplacement = true ;
    
    if (pos.re>anciennePos.re && pos.im>anciennePos.im)
        mouv = HD ;
    else if (pos.re>anciennePos.re && pos.im<anciennePos.im)
        mouv = BD ;
    else if (pos.re<anciennePos.re && pos.im>anciennePos.im)
        mouv = HG ;
    else if (pos.re<anciennePos.re && pos.im<anciennePos.im)
        mouv = BG ;
    else if (pos.re>anciennePos.re)
        mouv = D ;
    else if (pos.re<anciennePos.re)
        mouv = G ;
    else if (pos.im>anciennePos.im)
        mouv = H ;
    else if (pos.im<anciennePos.im)
        mouv = B ;
    anciennePos = pos ;
}

void Corps::Heal(const unsigned int pvBonus)
{
    pv += pvBonus ;
}

void Corps::healPerso()
{
    pv = pvmax;
}

void Corps::upAttackPerso()
{
    degat += 10;
}

void Corps::upPV()
{
    pvmax += 50;
}


bool Corps::estMort() const
{
    return pv <= 0;
}

bool Corps::estEnDeplacement() const 
{
    return Deplacement ;
}

void Corps::testRegression ()
{
    // Test distance
    std::cout<<"Test de la fonction distance"<<std::endl;
    pos = NBComplex(2,4);
    NBComplex pts2 (9,1) ;
    assert(pos.distance(pts2) > 7.61);
    std::cout<<"Test terminé (OK!)"<<std::endl;

    Corps co;
    // Test du constructeur par défaut
    std::cout<<"Test du constructeur par défaut (pv:"<<co.getPV()<<
                ",degat:"<<co.getDegat()<<",vitesse:"<<co.getVitesse()<<
                ",vitesseAtt:"<<co.getVitesseAtt()<<",pos("<<co.pos.re<<","<<co.pos.im<<"))"<<std::endl;
    assert(estMort() == false);
    assert(co.getPV() == 500);
    assert(co.getDegat() == 80);
    assert(co.getVitesse() == 3);
    assert(co.getVitesseAtt() == 1);
    assert(co.pos.re == 0);
    assert(co.pos.im == 0);
    std::cout<<"Test terminée(OK!)"<<std::endl;

    // Test du constructeur avec paramètres
    co = Corps(300, 100, 4, 2, 50, 60);
    std::cout<<"Test du constructeur avec paramètres (pv:"<<co.getPV()<<
                ",degat:"<<co.getDegat()<<",vitesse:"<<co.getVitesse()<<
                ",vitesseAtt:"<<co.getVitesseAtt()<<",pos("<<co.pos.re<<","<<co.pos.im<<"))"<<std::endl;
                
    assert(estMort() == false);
    assert(co.getPV() == 300);
    assert(co.getDegat() == 100);
    assert(co.getVitesse() == 4);
    assert(co.getVitesseAtt() == 2);
    assert(co.pos.re == 50);
    assert(co.pos.im == 60);
    std::cout<<"Test terminée(OK!)"<<std::endl;

}

