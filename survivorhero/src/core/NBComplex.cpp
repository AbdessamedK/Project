#include"NBComplex.h"
#include <math.h>
#include<iostream>
#include<cassert>

NBComplex::NBComplex () {re=0;im=0;}
NBComplex::NBComplex (float _re, float _im) {re=_re; im=_im;}
NBComplex::~NBComplex () {re=0; im=0;}

std::istream& operator>> (std::istream& flux, NBComplex& c)
{
    std::cout<< "Donnez le nombre complexe" <<std::endl ;
    flux>> c.re >> c.im ;
    return flux ;
}

std::ostream& operator<< (std::ostream& flux, const NBComplex& c)
{
    if (c.im>0)
        flux<< c.re << " + " << c.im <<std::endl ;
    else 
        flux<< c.re << " - " << c.im <<std::endl ;
    return flux ;
}

NBComplex NBComplex::operator+ (const NBComplex& c) const
{
    NBComplex res ;
    res.re = re + c.re ;
    res.im = im + c.im ;
    return res ;
}

NBComplex NBComplex::operator- (const NBComplex& c) const
{
    NBComplex res ;
    res.re = re - c.re ;
    res.im = im - c.im ;
    return res ;
}

NBComplex NBComplex::operator* (const NBComplex& c) const 
{
    NBComplex res ;
    res.re = re*c.re - im*c.im ;
    res.im = re*c.im + im*c.re ;
    return res ;
}

NBComplex NBComplex::operator* (const float lambda) const
{
    return NBComplex(lambda * re, lambda * im);
}

NBComplex NBComplex::complexExp (const float r, const float theta) const
{
    return NBComplex(r*cos(theta), r*sin(theta));
}

NBComplex NBComplex::rotate (const NBComplex & c , float theta_deg) const
{
    return (NBComplex(re,im) - c) * complexExp(1.0,theta_deg) + c;
}

float NBComplex::distance (const NBComplex & nb) const
{
    return sqrt((re-nb.re)*(re-nb.re) + (im-nb.im)*(im-nb.im));
}

void NBComplex::testRegression () const 
{
    NBComplex res ;
    assert(res.re==0);
    assert(res.im==0);
    std::cout<< "Affichage du complexe (0,0) : " << res <<std::endl ;
    char good ;
    std::cout<< "Affichage bon ? o/n" <<std::endl ;
    std::cin>> good ;
    assert(good=='o') ;

    good = 'f' ;
    NBComplex res1(10,10);
    std::cout<< "Affichage du complexe (10,10) : " << res1 <<std::endl ;
    std::cout<< "Affichage bon ? o/n" <<std::endl ;
    std::cin>> good ;
    assert(good=='o') ;

    NBComplex res2 = res + res1 ;
    assert(res2.re==10);
    assert(res2.im==10);

    res.re = 5 ; res.im = 5 ;

    res2 = res1 - res ;
    assert(res2.re==5);
    assert(res2.im==5);

    std::cout<< "Donnez 5 et -10 pour un complexe " <<std::endl ;
    std::cin>> res2 ;
    assert(res2.re==5);
    assert(res2.im==-10);

    res1.re = 10 ; res1.im = -5 ;
    res2 = res * res1 ; 
    assert(res2.re==75);
    assert(res2.im==25);

    float lambda = 2.0;


    res = NBComplex (1.0,2.0);
    res1 = res * lambda;
    assert(res1.re == 2.0);
    assert(res1.re == 6.0);




}