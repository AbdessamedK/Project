#include "winTxt.h"
#include "txtMonde.h"

int main ( int argc, char** argv ) {
    char rep ;
    
    std::cout<< "Commencer ? (o\\n)" <<std::endl ;
    std::cin>> rep ;

    if (rep=='o')
    {
        termClear();
        Monde m(true) ;
	    txtBoucle(m);
        termClear();
    }
    
	return 0;
}