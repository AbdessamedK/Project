#include"NBComplex.h"
#include"Pouvoir.h"
#include"Personnage.h"
#include"Corps.h"
#include"Monde.h"
#include"Aura.h"
#include"Bouclier.h"
#include"Terrain.h"
#include<iostream>

int main (void)
{
    /*NBComplex c ;
    c.testRegression() ; */
    //Pouvoir p ;
    //p.testRegression() ;

    // Corps a;
    // a.testRegression();

    //Personnage pers;
    //pers.testRegression();
    NBComplex centre(0,0);
    Aura a(centre);
    a.testRegression();

    Bouclier b(centre);
    b.testRegression();

    
   // Monde m ;
    //m.testRegression() ;

    //Terrain t;
    //t.testRegression();

    return 0 ;
}