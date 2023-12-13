#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include "winTxt.h"
#include<cassert>

#include "../core/Monde.h"

void txtAff(WinTXT & win, Monde & m) {

	win.clear();
	
    // Affichage des murs
    for (unsigned int x=0; x<100; ++x)
    {
        win.print(x,0,'#') ; 
        win.print(x,29,'#') ; 
    }
    for (unsigned int y=0; y<30; ++y)
    {
        win.print(0,y,'#');
        win.print(99,y,'#');
    } 

    // Affichage du perso
    win.print(m.perso.getPos().re,m.perso.getPos().im,'P') ;
	// Affichage des monstres
    for (unsigned int i=0; i<m.n[m.getNiveauActu()].monstres.size(); ++i)
	{
		win.print(m.n[m.getNiveauActu()].monstres[i].getPos().re,m.n[m.getNiveauActu()].monstres[i].getPos().im,'M'); 
	}
        
	
	// Affichage des particules
	std::vector<Projectile> pos ;
	m.perso.pouvoirs[0]->getPos(pos) ;
	NBComplex tab[pos.size()] ;
	for (unsigned int i=0; i<pos.size(); ++i)
	{
		tab[i] = pos[i].pos ;
	}
	for (unsigned int i=0; i<pos.size(); ++i)
	{
		win.print(tab[i].re,tab[i].im,'*') ;
	}

	// std::vector<NBComplex> pos1 ;
	// m.perso.pouvoirs[1]->getPos(pos1);
	// for(unsigned int i = 0 ; i < 3; i++)
	// {
	// 	win.print(pos1[i].re,pos1[i].im,'A') ;
	// }

	win.draw();
}

void txtBoucle (Monde & m) {
	// Creation d'une nouvelle fenetre en mode texte
	// => fenetre de dimension et position (WIDTH,HEIGHT,STARTX,STARTY)
    WinTXT win (100,30);

	bool ok = true;
	int c;

	do {
	    txtAff(win,m);

        #ifdef _WIN32
        Sleep(100);
		#else
		usleep(100000);
        #endif // WIN32

		//jeu.actionsAutomatiques();

		c = win.getCh();
		switch (c) {
			case 'o':
				m.updateTxt('o');
				ok = !m.n[m.getNiveauActu()].estFini() ;
				break;
			case 'l':
				m.updateTxt('l');
				ok = !m.n[m.getNiveauActu()].estFini() ;
				break;
			case 'k':
				m.updateTxt('k');
				ok = !m.n[m.getNiveauActu()].estFini() ;
				break;
			case 'm':
				m.updateTxt('m');
				ok = !m.n[m.getNiveauActu()].estFini() ;
				break; 
			case 'q':
				ok = false;
				break;
			default : 
				m.updateTxt('a');
				ok = !m.n[m.getNiveauActu()].estFini() ;
				break ;
		}

	} while (ok);

}