#include"SDLMonde.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<cassert>
#include<string.h>
#include<fstream>

using namespace std;

void SDLMonde::init_terrain()
{
    m_surface[0] = IMG_Load("data/Tilesets/tilesets_map1.png");
    if(!m_surface[0]){
        cout <<  "Erreur : " << IMG_GetError() << endl;
        exit(1);
    }
    m_surface[1] = IMG_Load("data/Tilesets/Tileset-Terrain2.png");
    if(!m_surface[1]) {
        cout <<  "Erreur : " << IMG_GetError() << endl;
        exit(1);
    }
    m_surface[2] = IMG_Load("data/Tilesets/map3.png");
    if(!m_surface[2]) {
        cout <<  "Erreur : " << IMG_GetError() << endl;
        exit(1);
    }
    for (unsigned int i=0; i<3; i++){
         m_texture[i] = SDL_CreateTextureFromSurface(m_renderer, m_surface[i]); // Création de la texture à partir de l'image
    }
}

void SDLMonde::affiche_terrain(int t)
{
    bool check;
    int taille;
    int nbCalques,nbColonnes,nbLignes;
    int nbLu = 0;

    string terrain = "data/Terrain/terrain";
    terrain += to_string(t) + ".txt";

    if (terrain == "data/Terrain/terrain1.txt"){check = true;}
    if (check) {taille = 16;}
    else taille = 32;

    ifstream ficTerrain(terrain);
    if (!ficTerrain){
        cout << "Erreur lors de l'ouverture du fichier .txt du terrain" << endl;
        exit(1);
    }

    ficTerrain >> nbCalques;
    ficTerrain >> nbColonnes;
    ficTerrain >> nbLignes;
    int nbCase = nbCalques * 15 * 15;
    unsigned int tabTerrain[nbCase];

    while (ficTerrain >> tabTerrain[nbLu] && nbLu < nbCase) {
            nbLu++;
    }
    
    SDL_Rect src_t;
    for (int nbC = 0; nbC < nbCalques; nbC ++){
        for (int i=0;i<15;i++){
            for (int j=0;j<15;j++){
                if (tabTerrain[(nbC * 15 * 15)+i*15 + j] != 0){
                    int indice = tabTerrain[(nbC * 15 * 15)+i*15 + j] - 1;
                    int px = indice % nbColonnes;
                    int py = indice / nbColonnes;
                    src_t = {px*taille, py*taille, taille,taille};
                    m.terrain.appliquerTexture(m_texture[t-1], m_renderer, &src_t, i, j);
                }
                else continue;
            }
        }
    }
    ficTerrain.close();
}

void SDLMonde::init_vector(vector<unsigned int>& tab)
{
    int niveauActu = m.getNiveauActu() ;
    int a = 0 ;
    for (unsigned int i=0; i<m.n[niveauActu].monstres.size(); ++i)
        tab.push_back(a) ;
}

void SDLMonde::afficherInit()
{
    init_vector(lastframeMonstreAttack) ;
    init_vector(lastframeMonstreStatic) ;
    texture_arrow = nullptr ;
    texture_aura = nullptr ;
    texture_bouclier = nullptr ;
    texture_rocket = nullptr ;
    interface = nullptr ;
    vie_joueur = nullptr ;
    progression_monde = nullptr ;
    musique = nullptr ;
    fondPerdu = nullptr;
    boutonQuitter = nullptr;
    boutonRetour = nullptr;
    imagefondTexture = nullptr;
    fondTexture = nullptr; 
    boutonJouerTexture = nullptr;
    boutonOptionTexture = nullptr;
    boutonQuitterTexture = nullptr;
    niveauActu = m.getNiveauActu() ;
    for (unsigned int i=0; i<8; ++i)
    {
        texture_static[i] = nullptr ;
        texture_run[i] = nullptr ;
        if (i < 3){
            m_texture[i] = nullptr;
            m_surface[i] = nullptr;
            interface_joueur[i] = nullptr ;
        }
    }
    for (unsigned int i=0; i<16; ++i)
    {
        texture_attack[i] = nullptr ;
        
        monstre_basique_run[i] = nullptr ;
        monstre_basique_attack[i] = nullptr ;

        monstre_rouge_run[i] = nullptr ;
        monstre_rouge_attack[i] = nullptr ;

        monstre_fox_run[i] = nullptr ;
        monstre_fox_attack[i] = nullptr ;

        monstre_robot_run[i] = nullptr ;
        monstre_robot_attack[i] = nullptr ;

        monstre_spider_run[i] = nullptr ;
        monstre_spider_attack[i] = nullptr ;

        boss_bleu_run[i] = nullptr ;
        boss_bleu_attack1[i] = nullptr ;
        boss_bleu_attack2[i] = nullptr ;

        boss_dragon_run[i] = nullptr ;
        boss_dragon_attack1[i] = nullptr ;
        boss_dragon_attack2[i] = nullptr ;

        boss_serpent_run[i] = nullptr ;
        boss_serpent_attack1[i] = nullptr ;
        boss_serpent_attack2[i] = nullptr ;
    }
        
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;SDL_Quit();exit(1);
    }
    
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        cout << "Erreur lors de l'initialisation du son de la SDL : " << Mix_GetError() << endl;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( !(IMG_Init(imgFlags) & imgFlags)) {
        cout << "SDL_m_image could not initialize! SDL_m_image Error: " << IMG_GetError() << endl;SDL_Quit();exit(1);
    }

    int _dimx, _dimy ;
    _dimx = 1000 ;
    _dimy = 800 ;

    m_window = SDL_CreateWindow("SurvivorHero!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _dimx, _dimy, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE) ;
    if (m_window == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl; SDL_Quit(); exit(1);
    }

    m_renderer = SDL_CreateRenderer(m_window,-1,SDL_RENDERER_ACCELERATED);

    SDL_RenderClear(m_renderer);
    estLance = true;
    visitOption = false;
    texture_partie = false;
    ecran = ecranMenu;
}

void SDLMonde::afficherMenu()
{
    musique = Mix_LoadMUS("data/Musique/menuMusique.mp3"); //Chargement de la 
    if (musique == NULL) {cerr << "Erreur lors de l'intialisation de la musique ecran Menu" << endl;}
    else {Mix_PlayMusic(musique, -1);}//Jouer infiniment la musique}

    // Chargement de l'image de fond
    SDL_Surface* imagefond = IMG_Load("data/Menu/image_fond.png");
    if(!imagefond){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    fondTexture = SDL_CreateTextureFromSurface(m_renderer, imagefond);
    SDL_FreeSurface(imagefond);


    // Chargement de l'image du bouton jouer
    SDL_Surface* boutonJouer = IMG_Load("data/Menu/bouton_jouer.png");
    if(!boutonJouer){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    boutonJouerTexture = SDL_CreateTextureFromSurface(m_renderer, boutonJouer);
    SDL_FreeSurface(boutonJouer);
    SDL_Rect boutonJouerRect = {(1000/2) - 200/2, 300, 200, 50};

    // Chargement de l'image du bouton options
    SDL_Surface* boutonOption = IMG_Load("data/Menu/bouton_option.png");
    if(!boutonOption){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    boutonOptionTexture = SDL_CreateTextureFromSurface(m_renderer, boutonOption);
    SDL_FreeSurface(boutonOption);
    SDL_Rect boutonOptionRect = {(1000/2) - 200/2, 400, 200, 50};  

    // Chargement de l'image du bouton quitter
    SDL_Surface* boutonQuitter = IMG_Load("data/Menu/bouton_quitter.png");
    if(!boutonQuitter){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    boutonQuitterTexture = SDL_CreateTextureFromSurface(m_renderer, boutonQuitter);
    SDL_FreeSurface(boutonQuitter);
    SDL_Rect boutonQuitterRect = {(1000/2) - 200/2, 500, 200, 50};  


    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        // Gestion des événements
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    if (musique != NULL){
                        Mix_HaltMusic();
                        Mix_FreeMusic(musique); //Libération de la musique
                    }
                    ecran = ecranFin;
                    break;
               case SDL_MOUSEBUTTONDOWN:
                    // Gestion des clics sur les boutons
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > boutonJouerRect.x && x < boutonJouerRect.x + boutonJouerRect.w && y > boutonJouerRect.y && y < boutonJouerRect.y + boutonJouerRect.h)
                    {
                        // Clic sur le bouton Play
                        printf("Jouer\n");
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        texture_partie = true;
                        ecran = ecranJeu;
                        quit = true;
                    }
                    else if (x > boutonOptionRect.x && x < boutonOptionRect.x + boutonOptionRect.w && y > boutonOptionRect.y && y < boutonOptionRect.y + boutonOptionRect.h)
                    {
                        // Clic sur le bouton Option
                        printf("Options\n");
                        ecran = ecranOption;
                        quit = true;
                    }
                    else if (x > boutonQuitterRect.x && x < boutonQuitterRect.x + boutonQuitterRect.w && y > boutonQuitterRect.y && y < boutonQuitterRect.y + boutonQuitterRect.h){
                        // Clic sur le bouton Exit
                        printf("Quitter\n");
                        ecran = ecranFin;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        quit = true;
                    }
                    break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE :
                        quit = true;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        ecran = ecranFin;
                        break ;
                    default:
                        break;
                }
            } 
        }
        // Affichage du fond d'écran
        SDL_RenderCopy(m_renderer, fondTexture, NULL, NULL);
        // Affichage des boutons
        SDL_RenderCopy(m_renderer, boutonJouerTexture, NULL, &boutonJouerRect);
        SDL_RenderCopy(m_renderer, boutonOptionTexture, NULL, &boutonOptionRect);
        SDL_RenderCopy(m_renderer, boutonQuitterTexture, NULL, &boutonQuitterRect);

        // Mise à jour de l'affichage
        SDL_RenderPresent(m_renderer);
    }

}

void SDLMonde::afficheOption()
{
    visitOption = true;
    // Chargement de l'image
    SDL_Surface* imagefond = IMG_Load("data/Menu/fond_option.png");
    if(!imagefond){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    imagefondTexture = SDL_CreateTextureFromSurface(m_renderer, imagefond);
    SDL_FreeSurface(imagefond);

    SDL_Rect boutonRetour = {23, 48, 102,87};


    bool quit = false;
    SDL_Event event;

    while (!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT :
                    estLance = false;
                    ecran = ecranFin;
                    quit = true;
                break;
                case SDL_MOUSEBUTTONDOWN :
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x > boutonRetour.x && x < boutonRetour.x + boutonRetour.w && y > boutonRetour.y && y < boutonRetour.y + boutonRetour.h){
                        printf("Retour\n");
                        ecran = ecranMenu;
                        quit = true;
                    }
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE :
                        quit = true;
                        ecran = ecranMenu;
                        break ;
                    default:
                        break;
                }
            } 
        }
        SDL_RenderCopy(m_renderer, imagefondTexture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }

}

void SDLMonde::affichePerdu()
{
    musique = Mix_LoadMUS("data/Musique/musiquePerdu.mp3"); //Chargement de la musique
    if (musique == NULL) {cerr << "Erreur lors de l'intialisation de la musique ecran Perdu" << endl;}
    else {Mix_PlayMusic(musique, -1);}

    // Chargement de l'image fond
    SDL_Surface* image_fond;
    image_fond = IMG_Load("data/Menu/fondPerdu.jpg");
    if (!image_fond) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    fondPerdu = SDL_CreateTextureFromSurface(m_renderer, image_fond);
    SDL_FreeSurface(image_fond);

    // Chargement du bouton Retour
    image_fond = IMG_Load("data/Menu/bouton_retour.png");
    if (!image_fond) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    boutonRetour = SDL_CreateTextureFromSurface(m_renderer, image_fond);
    SDL_FreeSurface(image_fond);
    SDL_Rect boutonRetourRect = {310, 512, 184, 40};

    // Chargement du bouton Quitter
    image_fond = IMG_Load("data/Menu/bouton_quitter.png");
    if (!image_fond) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    boutonQuitter = SDL_CreateTextureFromSurface(m_renderer, image_fond);
    SDL_FreeSurface(image_fond);
    SDL_Rect boutonQuitterRect = {528, 512, 184, 40};

    bool quit = false;
    SDL_Event event;

    while (!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT :
                    estLance = false;
                    ecran = ecranFin;
                    quit = true;
                break;
                case SDL_MOUSEBUTTONDOWN :
                     int x = event.button.x;
                     int y = event.button.y;
                     if (x > boutonRetourRect.x && x < boutonRetourRect.x + boutonRetourRect.w && y > boutonRetourRect.y && y < boutonRetourRect.y + boutonRetourRect.h){
                        printf("Retour\n");
                        ecran = ecranMenu;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        quit = true;
                     }
                     if (x > boutonQuitterRect.x && x < boutonQuitterRect.x + boutonQuitterRect.w && y > boutonQuitterRect.y && y < boutonQuitterRect.y + boutonQuitterRect.h){
                        printf("Quitter\n");
                        ecran = ecranFin;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        quit = true;
                     }
                 break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE :
                        estLance = false;
                        quit = true;
                        ecran = ecranFin;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        break ;
                    default:
                        break;
                }
            } 
        }
        SDL_RenderCopy(m_renderer, fondPerdu, NULL, NULL);
        SDL_RenderCopy(m_renderer, boutonRetour,NULL, &boutonRetourRect);
        SDL_RenderCopy(m_renderer, boutonQuitter, NULL, &boutonQuitterRect);
        SDL_RenderPresent(m_renderer);
    }

}

void SDLMonde::initBonus()
{

    // Initialisation des rectangles pour le choix du bonus
    choix1 = {75, 314, 262, 182};
    choix2 = {378, 314, 262, 182};
    choix3 = {678, 314, 262, 182};
    // Chargement de l'image aura
    SDL_Surface* image;
    image = IMG_Load("data/Bonus/aura.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[0] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[0] pour l'aura
    SDL_FreeSurface(image);

    // Chargement de l'image bouclier
    image = IMG_Load("data/Bonus/bouclier.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[1] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[1] pour le bouclier
    SDL_FreeSurface(image);
    
    // Chargement de l'image heal
    image = IMG_Load("data/Bonus/heal.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[2] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[2] pour le heal
    SDL_FreeSurface(image);

    // Chargement de l'image multiTire
    image = IMG_Load("data/Bonus/multiTire.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[3] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[3] pour le multi
    SDL_FreeSurface(image);

    // Chargement de l'image ricoche
    image = IMG_Load("data/Bonus/ricoche.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[4] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[4] pour le ricoche
    SDL_FreeSurface(image);

    // Chargement de l'image upAttack
    image = IMG_Load("data/Bonus/upAttack.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[5] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[5] pour le upAttack
    SDL_FreeSurface(image);

    // Chargement de l'image upPv
    image = IMG_Load("data/Bonus/upPv.png");
    if (!image) {cerr <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    bonus[6] = SDL_CreateTextureFromSurface(m_renderer, image); //bonus[6] pour le upPv
    SDL_FreeSurface(image);
}

void SDLMonde::afficheBonus(int tabChoix[3])
{
    choix1 = {75, 314, 262, 182};
    choix2 = {378, 314, 262, 182};
    choix3 = {678, 314, 262, 182};
    if (m.perso.pouvoirs.size() == 1){ // si le pouvoir Aura n'est pas débloqué
        cx1 = 0;
        cx2 = (rand()%5) + 2;
        cx3 = (rand()%5) + 2;
        while (cx3 == cx2) {cx3 = (rand()%5) + 2;} 
        SDL_RenderCopy(m_renderer, bonus[cx1], NULL, &choix1);
        SDL_RenderCopy(m_renderer, bonus[cx2], NULL, &choix2);
        SDL_RenderCopy(m_renderer, bonus[cx3], NULL, &choix3);
        tabChoix[0] = cx1;
        tabChoix[1] = cx2;
        tabChoix[2] = cx3;
    }
    else if (m.perso.pouvoirs.size() == 2 ){ // si le pouvoir Aura et déja débloqué
        cx1 = 1;
        cx2 = (rand()%5) + 2;
        cx3 = (rand()%5) + 2;
        while (cx3 == cx2) {cx3 = (rand()%5) + 2;} 
        SDL_RenderCopy(m_renderer, bonus[cx1], NULL, &choix1);
        SDL_RenderCopy(m_renderer, bonus[cx2], NULL, &choix2);
        SDL_RenderCopy(m_renderer, bonus[cx3], NULL, &choix3);
        tabChoix[0] = cx1;
        tabChoix[1] = cx2;
        tabChoix[2] = cx3;
    }
    else if (m.perso.pouvoirs.size() == 3){
        cx1 = (rand()%5) + 2;
        cx2 = (rand()%5) + 2;
        while (cx2 == cx1){cx2 = (rand()%5) + 2;}
        cx3 = (rand()%5) + 2;
        while (cx3 == cx2 || cx3 == cx1) {cx3 = (rand()%5) + 2;}
        SDL_RenderCopy(m_renderer, bonus[cx1], NULL, &choix1);
        SDL_RenderCopy(m_renderer, bonus[cx2], NULL, &choix2);
        SDL_RenderCopy(m_renderer, bonus[cx3], NULL, &choix3);
        tabChoix[0] = cx1;
        tabChoix[1] = cx2;
        tabChoix[2] = cx3;
    }
}

void SDLMonde::appliqueBonus(int b)
{
    switch (b)
    {
        case 0 :
            m.perso.ajouterPouvoir(new Aura(m.perso.getPos()));
            cout << "ajout de l'aura" << endl;
        break;
        case 1 :
            m.perso.ajouterPouvoir(new Bouclier(m.perso.getPos()));
            cout << "ajout du bouclier" << endl;
        break;
        case 2 :
            m.perso.heal();
            cout << "heal du perso " << endl;
        break;
        case 3 : 
            m.perso.activeTirMultiple();
            cout << "multi tir" << endl;
        break;
        case 4 : 
            m.perso.activeRicoche();
            cout << "ricoche" << endl;
        break;
        case 5 :
            m.perso.upAttaque();
            cout << "upAttack" << endl;
        break;
        case 6 :
            m.perso.upPvMax();
            cout << "uPPV" << endl;
        break;
    }
}

void TextureFromSprite(SDL_Renderer * m_renderer, SDL_Rect rect[], SDL_Texture * texture[], string filename, int nbimage_largeur, int nbimage_longueur, int nbdirection, int dimy, int dimx)
{
    for (int i=0; i<nbimage_longueur; ++i)
        for (int j=0; j<nbimage_largeur; ++j)
            rect[i*nbimage_largeur+j] = {j*dimx,i*dimy,dimx,dimy} ;

    SDL_Surface * sprite[nbdirection] ;

    string fname ;

    for (unsigned int i=0; i<nbdirection; ++i)
    {
        fname = filename +  to_string(i) + ".png" ;
        sprite[i] = IMG_Load(fname.c_str()) ;
        if (sprite[i]==nullptr)
        {
            cout<< "Error: cannot load " << fname <<endl ;
            SDL_Quit() ;
            exit(1) ;
        }
        texture[i] = SDL_CreateTextureFromSurface(m_renderer,sprite[i]) ;
    }
}

void SDLMonde::perso_animation_init(SDL_Rect rectStatic[], SDL_Rect rectRun[], SDL_Rect rectAtt[])
{
    TextureFromSprite(m_renderer,rectStatic,texture_static,"data/Perso/Idle/Idle_",8,5,8,180,180) ;
    TextureFromSprite(m_renderer,rectRun,texture_run,"data/Perso/Run/Run_",4,5,8,180,180) ;
    TextureFromSprite(m_renderer,rectAtt,texture_attack,"data/Perso/Attack/Attack_",6,4,16,180,180) ;
}

int getDirection (const NBComplex &perso, const NBComplex &monstre)
{
    double angle = atan2(monstre.im-perso.im, monstre.re-perso.re) * (180 / M_PI) * -1 ;
    if ( (angle<=180 && angle>=180-11.25) || (angle>=-180 && angle<=-180+11.25) ) 
        return 15 ;

    if ( ((angle >= -11.25) && (angle <= 11.25)) )
        return 0 ;

    int direction = 0 ;
    for (unsigned int i=0; i<8; ++i)
    {
        if ((abs(angle)>= i*22.5-11.25) && (abs(angle)<=i*22.5+11.25))
            direction = i ;
    }
    if (angle<0)
        direction+= 7 ;

    return direction ;
}

NBComplex EnnemiPlusProche (const NBComplex &posJoueur, std::vector<Monstre> tab_ennemi)
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

void SDLMonde::perso_animation(int &frame_actu_static, int &frame_actu_run, int &frame_actu_attack, SDL_Rect rectStatic[], SDL_Rect rectRun[], SDL_Rect rectAtt[], int direction)
{
    int largeur = 180 ;
    int hauteur = 180 ;
    SDL_Rect rectPos = {(int) m.perso.getPos().re-(largeur/2),(int) m.perso.getPos().im-(hauteur/2),largeur,hauteur};
    
    SDL_Rect rectstatic = rectStatic[frame_actu_static] ;
    SDL_Rect rectrun = rectRun[frame_actu_run] ;
    SDL_Rect rectatt = rectAtt[frame_actu_attack] ;

    if (!m.perso.estEnDeplacement())
    {
        if (m.n[niveauActu].nbMonstresVivants()<=0)
        {
            SDL_RenderCopy(m_renderer,texture_static[direction],&rectstatic,&rectPos) ;
            frame_actu_run = 0 ;
            frame_actu_attack = 0 ;
        
            if (SDL_GetTicks()-lastframe > 60)
            {
                frame_actu_static = (frame_actu_static+1)%40 ;
                lastframe = SDL_GetTicks() ;
            }
        }
        else 
        {
            NBComplex posproche = EnnemiPlusProche(m.perso.getPos(),m.n[niveauActu].monstres) ;
            int _direction = getDirection(m.perso.getPos(),posproche) ;

            SDL_RenderCopy(m_renderer,texture_attack[_direction],&rectatt, &rectPos) ;
            
            frame_actu_run = 0 ;
            frame_actu_static = 0 ;

            if (SDL_GetTicks()-lastframe > 25)
            {
                if (frame_actu_attack == 14)
                    m.perso.setPeutTirer(true) ;
                
                frame_actu_attack = (frame_actu_attack+1)%24 ;
                lastframe = SDL_GetTicks() ;
            } 
        }
    }
    else 
    {
        SDL_RenderCopy(m_renderer,texture_run[direction],&rectrun,&rectPos) ;
        frame_actu_static = 0 ;
        frame_actu_attack = 0 ;
        
        if (SDL_GetTicks()-lastframe > 40)
        {
            frame_actu_run = (frame_actu_run+1)%20 ;
            lastframe = SDL_GetTicks() ;
        }
    }
}

void SDLMonde::monstre_animation_init(SDL_Rect rectBRun[], SDL_Rect rectBAtt[], SDL_Rect rectRRun[], SDL_Rect rectRAtt[], SDL_Rect rectSAtt[], SDL_Rect rectBleuAtt2[])
{
    TextureFromSprite(m_renderer,rectBRun,monstre_basique_run,"data/Monstre/Basique/Run/Run_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBAtt,monstre_basique_attack,"data/Monstre/Basique/Attack/Attack_",6,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectRRun,monstre_rouge_run,"data/Monstre/Rouge/Run/Run_",4,5,16,256,256) ;
    TextureFromSprite(m_renderer,rectBAtt,monstre_rouge_attack,"data/Monstre/Rouge/Attack1/Attack_",6,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,monstre_fox_run,"data/Monstre/Fox/Run/Run_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,monstre_fox_attack,"data/Monstre/Fox/Attack/Attack_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBAtt,monstre_robot_run,"data/Monstre/Robot/Run/Run_",6,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectRAtt,monstre_robot_attack,"data/Monstre/Robot/Attack/Attack_",6,5,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,monstre_spider_run,"data/Monstre/Spider/Run/Run_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectSAtt,monstre_spider_attack,"data/Monstre/Spider/Attack/Attack_",5,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectRRun,boss_bleu_run,"data/Monstre/Bleu/Run/Run_",4,5,16,256,256) ;
    TextureFromSprite(m_renderer,rectRAtt,boss_bleu_attack1,"data/Monstre/Bleu/Attack1/Attack1_",6,5,16,256,256) ;
    TextureFromSprite(m_renderer,rectBleuAtt2,boss_bleu_attack2,"data/Monstre/Bleu/Attack2/Attack2_",8,5,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,boss_dragon_run,"data/Monstre/Dragon/Run/Run_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,boss_dragon_attack1,"data/Monstre/Dragon/Attack1/Attack1_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,boss_dragon_attack2,"data/Monstre/Dragon/Attack2/Attack2_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectSAtt,boss_serpent_run,"data/Monstre/Serpent/Run/Run_",5,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectBRun,boss_serpent_attack1,"data/Monstre/Serpent/Attack1/Attack1_",4,4,16,256,256) ;
    TextureFromSprite(m_renderer,rectSAtt,boss_serpent_attack2,"data/Monstre/Serpent/Attack2/Attack2_",5,4,16,256,256) ;
    
    SDL_Surface* bVie = IMG_Load("data/Interface/barre_vie.png");
    if(!bVie){cout <<  "Erreur : " << IMG_GetError() << endl; exit(1);}
    interface = SDL_CreateTextureFromSurface(m_renderer, bVie);
    SDL_FreeSurface(bVie);
}

void SDLMonde::monstre_animation(vector<unsigned int> &frame_actu_run, SDL_Rect rectRun[], vector<unsigned int> &frame_actu_att, SDL_Rect rectAtt[], SDL_Rect rectRRun[], SDL_Rect rectRAtt[], SDL_Rect rectSAtt[], SDL_Rect rectBleuAtt2[])
{
    int largeur, hauteur, _direction ;
    SDL_Rect rectPos, rectrun, rectatt ;
    float pvmax;
    if (m.n[niveauActu].nbMonstresVivants()>0)
    {
        for (unsigned int i=0; i<m.n[niveauActu].monstres.size(); ++i)
        {
            largeur = 256 ;
            hauteur = 256 ;
            rectPos = {(int) m.n[niveauActu].monstres[i].getPos().re-(largeur/2), (int) m.n[niveauActu].monstres[i].getPos().im-(hauteur/2),largeur,hauteur} ;
            
            _direction = getDirection(m.n[niveauActu].monstres[i].getPos(),m.perso.getPos()) ;
            
            switch (m.n[niveauActu].monstres[i].getType())
            {
                case Basique : 
                {
                    pvmax = 50*10;
                    rectrun = rectRun[frame_actu_run[i]] ;
                    rectatt = rectAtt[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        SDL_RenderCopy(m_renderer,monstre_basique_attack[_direction],&rectatt,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreAttack[i] > 70)
                        {
                            if (frame_actu_att[i]==23)
                                m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                            else if (frame_actu_att[i]==6 || frame_actu_att[i]==11)
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                            else 
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                            
                            frame_actu_att[i] = (frame_actu_att[i]+1)%24 ;
                            lastframeMonstreAttack[i] = SDL_GetTicks() ;
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,monstre_basique_run[_direction],&rectrun,&rectPos) ;
                        
                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 60)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%16 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
                case Rouge : 
                {
                    pvmax = 70*10;
                    rectrun = rectRRun[frame_actu_run[i]] ;
                    rectatt = rectAtt[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        SDL_RenderCopy(m_renderer,monstre_rouge_attack[_direction],&rectatt,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreAttack[i] > 120)
                        {
                            if (frame_actu_att[i]==23)
                                m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                            else if (frame_actu_att[i]==11)
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                            else 
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                            
                            frame_actu_att[i] = (frame_actu_att[i]+1)%24 ;
                            lastframeMonstreAttack[i] = SDL_GetTicks() ;
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,monstre_rouge_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 70)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%20 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }

                    break ;
                }
                case Fox : 
                {
                    pvmax = 30*10;
                    rectrun = rectRun[frame_actu_run[i]] ;
                    rectatt = rectRun[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        SDL_RenderCopy(m_renderer,monstre_fox_attack[_direction],&rectatt,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreAttack[i] > 40)
                        {
                            if (frame_actu_att[i]==15)
                                m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                            else if (frame_actu_att[i]>= 7 && frame_actu_att[i]<=12)
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                            else 
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;

                            frame_actu_att[i] = (frame_actu_att[i]+1)%16 ;
                            lastframeMonstreAttack[i] = SDL_GetTicks() ;
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,monstre_fox_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] >25)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%16 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
                case Robot :
                {
                    pvmax = 30*10;
                    rectrun = rectAtt[frame_actu_run[i]] ;
                    rectatt = rectRAtt[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        SDL_RenderCopy(m_renderer,monstre_robot_attack[_direction],&rectatt,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreAttack[i] > 70)
                        {
                            if (frame_actu_att[i]==7)
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                            else if (frame_actu_att[i]==29)
                                m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;

                            frame_actu_att[i] = (frame_actu_att[i]+1)%30 ;
                            lastframeMonstreAttack[i] = SDL_GetTicks() ;
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,monstre_robot_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 60)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%24 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
                case Spider : 
                {
                    pvmax = 40*10 ;
                    rectrun = rectRun[frame_actu_run[i]] ;
                    rectatt = rectSAtt[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        SDL_RenderCopy(m_renderer,monstre_spider_attack[_direction],&rectatt,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreAttack[i] > 50)
                        {
                            if (frame_actu_att[i]==19)
                                m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                            else if (frame_actu_att[i]==7)
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                            else 
                                m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;

                            frame_actu_att[i] = (frame_actu_att[i]+1)%20 ;
                            lastframeMonstreAttack[i] = SDL_GetTicks() ;
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,monstre_spider_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 45)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%16 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
                case Bleu :
                {
                    pvmax = 300*5 ;
                    rectrun = rectRRun[frame_actu_run[i]] ;
                    if (m.n[niveauActu].monstres[i].getAttaque()==0)
                        rectatt = rectRAtt[frame_actu_att[i]] ;
                    else 
                        rectatt = rectBleuAtt2[frame_actu_att[i]] ;
                    
                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        if (m.n[niveauActu].monstres[i].getAttaque()==0) // attaque 1 du boss
                        {
                            SDL_RenderCopy(m_renderer,boss_bleu_attack1[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 60)
                            {
                                if (frame_actu_att[i]==29)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]==9)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else 
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;

                                frame_actu_att[i] = (frame_actu_att[i]+1)%30 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        else // attaque 2 du boss 
                        {
                            SDL_RenderCopy(m_renderer,boss_bleu_attack2[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 70)
                            {
                                if (frame_actu_att[i]==39)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]==9 || frame_actu_att[i]==15 || frame_actu_att[i]==26)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else 
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                                
                                frame_actu_att[i] = (frame_actu_att[i]+1)%40 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,boss_bleu_run[_direction],&rectrun,&rectPos) ;
                        frame_actu_att[i] = 0 ;
                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 90)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%20 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                    }
                    break ;
                }
                case Dragon :
                {
                    pvmax = 300*5 ;
                    rectrun = rectRun[frame_actu_run[i]] ;
                    rectatt = rectRun[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        if (m.n[niveauActu].monstres[i].getAttaque()==0)
                        {
                            SDL_RenderCopy(m_renderer,boss_dragon_attack1[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 50)
                            {
                                if (frame_actu_att[i] == 15)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]>=6 && frame_actu_att[i]<=13)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else 
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                                frame_actu_att[i] = (frame_actu_att[i]+1)%16 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        else 
                        {
                            SDL_RenderCopy(m_renderer,boss_dragon_attack2[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 50)
                            {
                                if (frame_actu_att[i]==15)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]>=5 && frame_actu_att[i]<=12)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else 
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                                frame_actu_att[i] = (frame_actu_att[i]+1)%16 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,boss_dragon_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 50)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%16 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
                case Serpent :
                {
                    pvmax = 300*5 ;
                    rectrun = rectSAtt[frame_actu_run[i]] ;
                    if (m.n[niveauActu].monstres[i].getAttaque()==0)
                        rectatt = rectRun[frame_actu_att[i]] ;
                    else 
                        rectatt = rectSAtt[frame_actu_att[i]] ;

                    if (m.n[niveauActu].monstres[i].getPeutAttaquer())
                    {
                        if (m.n[niveauActu].monstres[i].getAttaque()==0)
                        {
                            SDL_RenderCopy(m_renderer,boss_serpent_attack1[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 50)
                            {
                                if (frame_actu_att[i]==15)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]>=8 && frame_actu_att[i]<=11)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                                frame_actu_att[i] = (frame_actu_att[i]+1)%16 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        else 
                        {
                            SDL_RenderCopy(m_renderer,boss_serpent_attack2[_direction],&rectatt,&rectPos) ;

                            if (SDL_GetTicks()-lastframeMonstreAttack[i] > 60)
                            {
                                if (frame_actu_att[i]==19)
                                    m.n[niveauActu].monstres[i].setPeutAttaquer(false) ;
                                else if (frame_actu_att[i]>=7 && frame_actu_att[i]<=11)
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(true) ;
                                else 
                                    m.n[niveauActu].monstres[i].setPeutEnlevePv(false) ;
                                frame_actu_att[i] = (frame_actu_att[i]+1)%20 ;
                                lastframeMonstreAttack[i] = SDL_GetTicks() ;
                            }
                        }
                        frame_actu_run[i] = 0 ;
                    }
                    else 
                    {
                        SDL_RenderCopy(m_renderer,boss_serpent_run[_direction],&rectrun,&rectPos) ;

                        if (SDL_GetTicks()-lastframeMonstreStatic[i] > 45)
                        {
                            frame_actu_run[i] = (frame_actu_run[i]+1)%20 ;
                            lastframeMonstreStatic[i] = SDL_GetTicks() ;
                        }
                        frame_actu_att[i] = 0 ;
                    }
                    break ;
                }
            }
            
            SDL_Rect bw = m.n[niveauActu].monstres[i].getMonstreBarreVie();
            SDL_Rect barre_vie = m.n[niveauActu].monstres[i].drawMonstreBarreVie(pvmax,bw.w, bw.h);
            bw.w = barre_vie.w;
            SDL_RenderCopy(m_renderer, interface, &barre_vie, &bw);
        }
    }
}

void SDLMonde::arrow_animation_init()
{
    SDL_Surface * temp ;
    temp = IMG_Load("data/Arrow/arrow.png") ;
    texture_arrow = SDL_CreateTextureFromSurface(m_renderer,temp) ;
}

void SDLMonde::bouclier_animation_init()
{
    SDL_Surface * temp = IMG_Load("data/Bouclier/bouclier.png") ;
    texture_bouclier = SDL_CreateTextureFromSurface(m_renderer,temp) ;
}

void SDLMonde::aura_animation_init(SDL_Rect rectAura[4])
{

    SDL_Surface * temp = IMG_Load("data/Aura/aura.png") ;
    texture_aura = SDL_CreateTextureFromSurface(m_renderer,temp) ;
    int dimx,dimy;
    dimx = 74;
    dimy = 79;
    for(int i = 0 ; i < 4; i++)
    {
        rectAura[i] = {i*74,0,dimx,dimy};
    }
}

void SDLMonde::arrow_animation()
{
    vector<Projectile> tabproj ;
    m.perso.pouvoirs[0]->getPos(tabproj) ;
    double angle ;
    SDL_Rect pos ;
    int dimy=531/12, dimx=1505/35 ;
    for (unsigned int i=0; i<tabproj.size(); ++i)
    {
        angle = atan2(tabproj[i].vecDir.im,tabproj[i].vecDir.re) * (180 / M_PI) ;
        if (angle<0)
            angle += 360 ;
        pos = {(int) tabproj[i].pos.re-dimx/2, (int) tabproj[i].pos.im-dimy/2,dimx,dimy} ;
        SDL_RenderCopyEx(m_renderer,texture_arrow,NULL,&pos,angle,NULL,SDL_FLIP_NONE);
    }
}

void SDLMonde::aura_animation(SDL_Rect rectAura[4], int & frame_actu)
{
    if(m.perso.pouvoirs.size() > 1)
    {
        vector<NBComplex> tabpos ;
        m.perso.pouvoirs[1]->getPos(tabpos);
        int longueur,largeur;
        longueur = largeur = m.perso.pouvoirs[1]->getLargeur();

        SDL_Rect hitbox[3];
        m.perso.pouvoirs[1]->getHitbox(hitbox);

        SDL_Rect rectpos{0,0,0,0};
        for(int i = 0 ; i < 3;i++)
        {
            rectpos = {(int) tabpos[i].re - largeur/2,  (int) tabpos[i].im - largeur/2,longueur,longueur};
            SDL_RenderCopy(m_renderer,texture_aura,&rectAura[frame_actu],&rectpos);
        }
        if(SDL_GetTicks() % 16 == 0)
        {
            frame_actu = (frame_actu +1) %4;
        }
    }
}

void SDLMonde::bouclier_animation()
{
    if(m.perso.pouvoirs.size() > 2)
    {
        vector<NBComplex> tabpos ;
        m.perso.pouvoirs[2]->getPos(tabpos);
        int largeur;
        largeur = m.perso.pouvoirs[2]->getLargeur();

        SDL_Rect hitbox[3];
        m.perso.pouvoirs[2]->getHitbox(hitbox);

        SDL_Rect rectpos{0,0,0,0};
        for(int i = 0 ; i < 3;i++)
        {
            rectpos = {(int) tabpos[i].re - largeur/2,  (int) tabpos[i].im - largeur/2,largeur,largeur};
            SDL_RenderCopy(m_renderer,texture_bouclier,NULL,&rectpos);
        }
    }
}

void SDLMonde::rocket_animation_init()
{
    SDL_Surface * temp ;
    temp = IMG_Load("data/Monstre/Robot/Rocket.png") ;
    texture_rocket = SDL_CreateTextureFromSurface(m_renderer,temp) ;
}

void SDLMonde::rocket_animation()
{
    int dimy=1523/30, dimx=1122/30 ;
    for (unsigned int i=0; i<m.n[niveauActu].monstres.size(); ++i)
    {
        if (m.n[niveauActu].monstres[i].getAffProj())
        {
            Projectile proj = m.n[niveauActu].monstres[i].getProj() ;
            double angle = atan2(proj.vecDir.im,proj.vecDir.re) * (180/M_PI) ;
            if (angle<0)
                angle += 360 ;
            SDL_Rect pos = {(int) proj.pos.re-dimx/2, (int) proj.pos.im-dimy/2,dimx,dimy} ;
            SDL_RenderCopyEx(m_renderer,texture_rocket,NULL,&pos,angle,NULL,SDL_FLIP_NONE) ;
        }
    }
}

void SDLMonde::init_interfaceJoueur()
{
    SDL_Surface * surface = IMG_Load("data/Interface/interface_monde1.png");
    if (!surface) {cerr << "Erreur : " << IMG_GetError() << endl; exit(1);}
    interface_joueur[0] = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("data/Interface/interface_monde2.png");
    if (!surface) {cerr << "Erreur : " << IMG_GetError() << endl; exit(1);}
    interface_joueur[1] = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("data/Interface/interface_monde3.png");
    if (!surface) {cerr << "Erreur : " << IMG_GetError() << endl; exit(1);}
    interface_joueur[2] = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("data/Interface/barre_vie.png");
    if (!surface) {cerr << "Erreur : " << IMG_GetError() << endl; exit(1);}
    vie_joueur = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("data/Interface/barre_progression.png");
    if (!surface) {cerr << "Erreur : " << IMG_GetError() << endl; exit(1);}
    progression_monde = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
}

void SDLMonde::afficheInterfaceJoueur()
{
    SDL_Rect stats_dest = {0,0,230,80};
    if (niveauActu >= 0 && niveauActu<=4){
        SDL_RenderCopy(m_renderer, interface_joueur[0], NULL, &stats_dest);
    }
    else if (niveauActu >= 5 && niveauActu<=9){
        SDL_RenderCopy(m_renderer, interface_joueur[1], NULL, &stats_dest);
    }
    else if (niveauActu >= 10 && niveauActu<=14){
        SDL_RenderCopy(m_renderer, interface_joueur[2], NULL, &stats_dest);
    }

    SDL_Rect vieJoueur = {78, 10, 140, 7};
    SDL_Rect barre_vieJoueur = m.perso.drawPersoBarreVie(vieJoueur.w, vieJoueur.h);
    vieJoueur.w = barre_vieJoueur.w;
    SDL_RenderCopy(m_renderer, vie_joueur, &barre_vieJoueur, &vieJoueur);

    SDL_Rect progMonde = {78, 30, 120, 7};
    int bw = (int)(((float)(niveauActu % 5) / 5.0) * (float)progMonde.w); // Calcule de la barre de vie en fonction de la santé actuelle du monstre
    SDL_Rect barre_progressionMonde = {0, 0, bw, progMonde.h};
    progMonde.w = barre_progressionMonde.w;
    SDL_RenderCopy(m_renderer, progression_monde, &barre_progressionMonde, &progMonde);
}

bool SDLMonde::niveauAChanger()
{
    return m.getNiveauActu() != niveauActu ;
}

void SDLMonde::afficherBoucle()
{
    initBonus();
    musique = Mix_LoadMUS("data/Musique/jeuMusique.mp3"); //Chargement de la musique
    if (musique == NULL) {cerr << "Erreur lors de l'intialisation de la musique ecran Jeu" << endl;}
    else {Mix_PlayMusic(musique, -1);} //Jouer infiniment la musique

    init_terrain();
    init_interfaceJoueur();
    
    
    SDL_Rect rectStatic[40] ;
    SDL_Rect rectRun[20] ;
    SDL_Rect rectAtt[24] ;
    SDL_Rect rectAura[4] ;
    perso_animation_init(rectStatic,rectRun,rectAtt);
    int frame_actu_static = 0 ;
    int frame_actu_run = 0 ;
    int frame_actu_attack = 0 ;
    int frame_actu_aura = 0;

    SDL_Rect rectMonstreBRun[16] ;
    SDL_Rect rectMonstreBAttack[24] ;
    SDL_Rect rectMonstreRRun[20] ;
    SDL_Rect rectMonstreRAtt[30] ;
    SDL_Rect rectMonstreSAtt[20] ;
    SDL_Rect rectBleuAtt2[40] ;
    monstre_animation_init(rectMonstreBRun,rectMonstreBAttack,rectMonstreRRun,rectMonstreRAtt,rectMonstreSAtt,rectBleuAtt2) ;
    vector <unsigned int> frame_actu_monstre_run ;
    vector <unsigned int> frame_actu_monstre_attack ;
    init_vector(frame_actu_monstre_run) ;
    init_vector(frame_actu_monstre_attack) ;

    arrow_animation_init() ;
    rocket_animation_init() ;
    aura_animation_init(rectAura);
    cout<<texture_aura<<endl;
    bouclier_animation_init();


    SDL_Event events; 
    bool quit = false;
    bool aChoisi = false;

    while (!quit) 
    {
        if (m.perso.getPV()<=0)
        {
            ecran = ecranPerdu ;
            quit = true ;
        }
        while (SDL_PollEvent(&events))
        {
            if (events.type == SDL_QUIT) {quit = true; Mix_HaltMusic(); Mix_FreeMusic(musique); ecran = ecranFin; }
            else if (events.type == SDL_KEYDOWN)
            {
                switch (events.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE :
                        quit = true ;
                        if (musique != NULL){
                            Mix_HaltMusic();
                            Mix_FreeMusic(musique); //Libération de la musique
                        }
                        ecran = ecranFin;
                        break ;
                }
            } 
        }
        if ((niveauActu == 3 && m.n[3].estFini()) || (niveauActu == 7 && m.n[7].estFini()) || (niveauActu == 10 && m.n[10].estFini()) || (niveauActu == 13 && m.n[13].estFini())){
                if(!aChoisi){
                    SDL_Event eventsBonus;
                    bool quitBonus = false;
                    while(!quitBonus){
                        while(SDL_PollEvent(&eventsBonus)){
                            if (eventsBonus.type == SDL_MOUSEBUTTONDOWN && eventsBonus.button.button == SDL_BUTTON_LEFT) {
                                int x = eventsBonus.button.x;
                                int y = eventsBonus.button.y;
                                if (x > choix1.x && x < choix1.x + choix1.w && y > choix1.y && y < choix1.y + choix1.h) {
                                    // Action correspondante au choix 1
                                    appliqueBonus(Choix[0]);
                                    aChoisi = true;
                                    quitBonus = true;
                                }
                                if (x > choix2.x && x < choix2.x + choix2.w && y > choix2.y && y < choix2.y + choix2.h) {
                                    // Action correspondante au choix 1
                                    appliqueBonus(Choix[1]);
                                    aChoisi = true;
                                    quitBonus = true;
                                }
                                if (x > choix3.x && x < choix3.x + choix3.w && y > choix3.y && y < choix3.y + choix3.h) {
                                    // Action correspondante au choix 1
                                    appliqueBonus(Choix[2]);
                                    aChoisi = true;
                                    quitBonus = true;
                                }
                            }
                    }
                }
            }
        }
        if (niveauAChanger())
        {
            init_vector(frame_actu_monstre_run) ;
            init_vector(frame_actu_monstre_attack) ;
            init_vector(lastframeMonstreAttack) ;
            init_vector(lastframeMonstreStatic) ;
            niveauActu = m.getNiveauActu() ;
            aChoisi = false ;
        }

        m.boucle() ;
        SDL_RenderClear(m_renderer);
        affiche_terrain(m.getTerrainActu());
        afficheInterfaceJoueur();
        perso_animation(frame_actu_static,frame_actu_run,frame_actu_attack,rectStatic,rectRun,rectAtt,m.perso.getMouv());
        monstre_animation(frame_actu_monstre_run,rectMonstreBRun,frame_actu_monstre_attack,rectMonstreBAttack,rectMonstreRRun,rectMonstreRAtt,rectMonstreSAtt,rectBleuAtt2) ;
        arrow_animation() ;
        rocket_animation() ;
        bouclier_animation();
        aura_animation(rectAura,frame_actu_aura);
        if ((niveauActu == 3 && m.n[3].estFini() && !aChoisi) || (niveauActu == 7 && m.n[7].estFini() && !aChoisi) || (niveauActu == 10 && m.n[10].estFini() && !aChoisi) || (niveauActu == 13 && m.n[13].estFini() && !aChoisi)){
           afficheBonus(Choix);
        }
        SDL_RenderPresent(m_renderer);
    }
}

void SDLMonde::afficherDetruit()
{
    cout << "destruction" << endl;
    cout << "fin du jeu" << endl;

    if(texture_partie){
        SDL_DestroyTexture(texture_arrow);
        cout << "arrow destroy " << endl;
        texture_arrow = nullptr;
        SDL_DestroyTexture(interface);
        interface = nullptr;
        cout << "interface destroy" << endl;
        SDL_DestroyTexture(vie_joueur);
        vie_joueur = nullptr;
        cout << "vie joueur destroy " << endl;
        SDL_DestroyTexture(progression_monde);
        progression_monde = nullptr;
        cout << "progression monde destroy " << endl;
        SDL_DestroyTexture(texture_rocket);
        texture_rocket = nullptr;
        cout << "rocket destroy " << endl;
        SDL_DestroyTexture(fondPerdu);
        fondPerdu = nullptr;
        SDL_DestroyTexture(boutonQuitter);
        boutonQuitter = nullptr;
        SDL_DestroyTexture(boutonRetour);
        boutonRetour = nullptr;
        for (unsigned int i=0; i<8; i++){
            SDL_DestroyTexture(texture_static[i]);
            texture_static[i] = nullptr;
            SDL_DestroyTexture(texture_run[i]) ;
            texture_run[i] = nullptr;
            if (i < 3){
                SDL_DestroyTexture(m_texture[i]);
                m_texture[i] = nullptr;
                SDL_FreeSurface(m_surface[i]);
                m_surface[i] = nullptr;
                SDL_DestroyTexture(interface_joueur[i]);
                interface_joueur[i] = nullptr;
            }
        }
        for (unsigned int i=0; i<16; ++i){
            SDL_DestroyTexture(texture_attack[i]) ;
            texture_attack[i] = nullptr;
            SDL_DestroyTexture(monstre_basique_run[i]) ;
            monstre_basique_run[i] = nullptr;
            SDL_DestroyTexture(monstre_basique_attack[i]) ;
            monstre_basique_attack[i] = nullptr;

            SDL_DestroyTexture(monstre_rouge_run[i]) ;
            monstre_rouge_run[i] = nullptr;
            SDL_DestroyTexture(monstre_rouge_attack[i]) ;
            monstre_rouge_attack[i] = nullptr;

            SDL_DestroyTexture(monstre_fox_run[i]) ;
            monstre_fox_run[i] = nullptr;
            SDL_DestroyTexture(monstre_fox_attack[i]) ;
            monstre_fox_attack[i] = nullptr;

            SDL_DestroyTexture(monstre_robot_run[i]) ;
            monstre_robot_run[i] = nullptr;
            SDL_DestroyTexture(monstre_robot_attack[i]) ;
            monstre_robot_attack[i] = nullptr;

            SDL_DestroyTexture(monstre_spider_run[i]) ;
            monstre_spider_run[i] = nullptr ;
            SDL_DestroyTexture(monstre_spider_attack[i]) ;
            monstre_spider_attack[i] = nullptr ;

            SDL_DestroyTexture(boss_bleu_run[i]) ;
            boss_bleu_run[i] = nullptr ;
            SDL_DestroyTexture(boss_bleu_attack1[i]) ;
            boss_bleu_attack1[i] = nullptr ;
            SDL_DestroyTexture(boss_bleu_attack2[i]) ;
            boss_bleu_attack2[i] = nullptr ;

            SDL_DestroyTexture(boss_dragon_run[i]) ;
            boss_dragon_run[i] = nullptr ;
            SDL_DestroyTexture(boss_dragon_attack1[i]) ;
            boss_dragon_attack1[i] = nullptr ;
            SDL_DestroyTexture(boss_dragon_attack2[i]) ;
            boss_dragon_attack2[i] = nullptr ;

            SDL_DestroyTexture(boss_serpent_run[i]) ;
            boss_serpent_run[i] = nullptr ;
            SDL_DestroyTexture(boss_serpent_attack1[i]) ;
            boss_serpent_attack1[i] = nullptr ;
            SDL_DestroyTexture(boss_serpent_attack2[i]) ;
            boss_serpent_attack2[i] = nullptr ;

            cout << "texture de la partie détruites " << endl;
    }
    if (visitOption){SDL_DestroyTexture(imagefondTexture); imagefondTexture = nullptr; cout << "Texture option détruite" << endl;}
    if (m.getNiveauActu() > 4){
        for (unsigned int i=0; i<7;i++){
            SDL_DestroyTexture(bonus[i]);
            bonus[i] = nullptr;
        }
    }

    SDL_DestroyTexture(fondTexture);
    fondTexture = nullptr; 

    SDL_DestroyTexture(boutonJouerTexture);
    boutonJouerTexture = nullptr;
    SDL_DestroyTexture(boutonOptionTexture);
    boutonOptionTexture = nullptr;
    SDL_DestroyTexture(boutonQuitterTexture);
    boutonQuitterTexture = nullptr;

    cout << "renderer delete " << endl;
    SDL_DestroyWindow(m_window);
    cout << "window destroy " << endl;
    Mix_CloseAudio();
    SDL_Quit();
    }
}

void SDLMonde::afficher()
{
    switch (ecran){
        case ecranMenu :
            afficherMenu();
            break;
        case ecranJeu :
            afficherBoucle();
            break;
        case ecranOption :
            afficheOption();
            break;
        case ecranPerdu :
            affichePerdu();
            break;
        case ecranFin :
            afficherDetruit() ;
            estLance = false;
            break;
    }
}
