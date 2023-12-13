#include <iostream>
#include "Niveau.h"


Niveau::Niveau(){}
Niveau::~Niveau(){}

void Niveau::ajouteMonstre(Monstre m)
{
    monstres.push_back(m);
}

int Niveau::nbMonstresVivants() const
{
    return monstres.size();
}

void Niveau::supprimeMonstreMorts()
{
    for(auto monstre = monstres.begin() ; monstre != monstres.end() ;)
        if(monstre->estMort())
            monstre = monstres.erase(monstre);
        else 
            ++monstre;
}

bool Niveau::estFini() const
{
    return nbMonstresVivants() == 0;
}

void Niveau::update(Personnage& p) 
{
    if (monstres.size()>0)
    {
        supprimeMonstreMorts();
        for(unsigned int i=0; i<monstres.size(); ++i)
            monstres[i].deplacementAuto(p) ;
    }
}