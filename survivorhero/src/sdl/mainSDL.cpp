#include"SDLMonde.h"

int main (void)
{
    srand(time(NULL)) ;
    SDLMonde m ;
    m.afficherInit();
    while(m.estLance){m.afficher() ;}
    return 0 ;
}