#ifndef _SDLMONDE_H
#define _SDLMONDE_H

#include<iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include"../core/Monde.h"
#include"../core/Bouclier.h"
#include"../core/Aura.h"

/// @brief Les différents écran du jeu
enum ecranAffiche {ecranMenu, ecranJeu, ecranOption, ecranPerdu, ecranFin};

/// @class SDLMonde
class SDLMonde 
{
    private : 

    // terrain
    /// @brief Surface pour stocker les textures du terrain
    SDL_Surface * m_surface[3];
    /// @brief Texture pour stocker les texture du terrain
    SDL_Texture * m_texture[3];
    /// @brief Niveau actuelle dans le jeu
    int niveauActu ;

    
    /// @brief Sprite de 8 directions quand le perso ne bouge pas
    SDL_Texture * texture_static[8];
    /// @brief Sprite de 8 direction de l'anim run du perso
    SDL_Texture * texture_run[8] ;
    /// @brief Sprite 16 direction du perso attaque
    SDL_Texture * texture_attack[16];
    
    /// @brief Sprite run de 16 direction du monstre basique
    SDL_Texture * monstre_basique_run[16];
    /// @brief Sprite attaque de 16 direction du monstre basique
    SDL_Texture * monstre_basique_attack[16];
    
    /// @brief Sprite run de 16 direction du monstre rouge
    SDL_Texture * monstre_rouge_run[16];
    /// @brief Sprite attaque de 16 direction du monstre rouge
    SDL_Texture * monstre_rouge_attack[16];

    /// @brief Sprite run de 16 direction du monstre renard
    SDL_Texture * monstre_fox_run[16];
    /// @brief Sprite attaque de 16 direction du monstre renard
    SDL_Texture * monstre_fox_attack[16];

    /// @brief Sprite run de 16 direction du monstre robot
    SDL_Texture * monstre_robot_run[16];
    /// @brief Sprite attaque de 16 direction du monstre robot
    SDL_Texture * monstre_robot_attack[16];

    /// @brief Sprite run de 16 direction de l'araigné
    SDL_Texture * monstre_spider_run[16];
    /// @brief Sprite attaque de 16 direction de l'araigné
    SDL_Texture * monstre_spider_attack[16];

    /// @brief Sprite run de 16 direction du boss Bleu
    SDL_Texture * boss_bleu_run[16];
    /// @brief Sprite attaque 1 de 16 direction du boss Bleu
    SDL_Texture * boss_bleu_attack1[16];
    /// @brief Sprite attaque 2 de 16 direction du boss Bleu
    SDL_Texture * boss_bleu_attack2[16];

    /// @brief Srite run de 16 direction du boss Dragon
    SDL_Texture * boss_dragon_run[16];
    /// @brief Sprite attaque 1 de 16 direction du boss Dragon
    SDL_Texture * boss_dragon_attack1[16];
    /// @brief Sprite attaque 2 de 16 direction du boss Dragon
    SDL_Texture * boss_dragon_attack2[16];

    /// @brief Sprite run de 16 direction du boss Serpent
    SDL_Texture * boss_serpent_run[16];
    /// @brief Sprite attaque 1 de 16 direction du boss Serpent
    SDL_Texture * boss_serpent_attack1[16];
    /// @brief Sprite attaque 2 de 16 direction du boss Serpent
    SDL_Texture * boss_serpent_attack2[16];

    /// @brief Image de la fleche
    SDL_Texture * texture_arrow ;

    /// @brief Image du missile pour le monstre robot
    SDL_Texture * texture_rocket ;

    /// @brief Sprite de 4 images pour l'aura du perso
    SDL_Texture * texture_aura;

    /// @brief Image du pouvoir Bouclier pour le perso
    SDL_Texture * texture_bouclier;
    
    /// @brief rendu de l'affichage graphique
    SDL_Renderer * m_renderer ;
    /// @brief fenetre sdl pour l'affichage graphique
    SDL_Window * m_window ;
    
    // menu
    /// @brief L'ecran à afficher
    ecranAffiche ecran;
    /// @brief La texture de l'image du fond du menu
    SDL_Texture* fondTexture;

    /// @brief La texture du bontonJouer situé sur le menu
    SDL_Texture* boutonJouerTexture;
    /// @brief la texture du boutonOption situé sur le menu
    SDL_Texture* boutonOptionTexture;
    /// @brief la texture du boutonQuitter situé sur le menu
    SDL_Texture* boutonQuitterTexture;

    // musique
    /// @brief la musique joué pendant l'éxécution du programme
    Mix_Music *musique;

    // interface 
    /// @brief la texture pour l'interface joueur
    SDL_Texture* interface;
    /// @brief Tableau de texture pour stocker les différentes interfaces des différents monde
    SDL_Texture* interface_joueur[3];
    /// @brief Texture pour la barre de vie du joueur
    SDL_Texture* vie_joueur;
    /// @brief Texture pour la barre de progression dans le monde
    SDL_Texture* progression_monde;

    // ecran Perdu
    /// @brief La texture pour l'image de fond de l'écran fin de jeu
    SDL_Texture* fondPerdu;
    /// @brief La texture pour le bouton quitter de l'écran fin de jeu
    SDL_Texture* boutonQuitter;
    /// @brief la texture pour le bouton retour de l'écran fin de jeu
    SDL_Texture* boutonRetour;

    // ecran Option
    /// @brief Indique si l'utilisateur à visiter la page option
    bool visitOption;
    /// @brief La texture pour l'image de fond de l'écran Option
    SDL_Texture* imagefondTexture;

    // Pour les bonus
    /// @brief Les textures pour les différents Bonus
    SDL_Texture* bonus[7];
    /// @brief Les rectangles qui correspond à la position de chaque Bonus sur l'écran
    SDL_Rect choix1, choix2, choix3;
    /// @brief Permet d'afficher des bonus aléatoirement
    int cx1, cx2, cx3;
    /// @brief Tableau pour stocker les différents Bonus qui ont été proprosé à l'utilisateur
    int Choix[3];

    /// @brief Monde a afficher 
    Monde m ;
    /// @brief entier pour stocker la derniere image affiché pour le perso
    int lastframe ;
    /// @brief tableau d'entier pour stocket le moment de la derniere image run affiché pour les monstres
    std::vector<unsigned int> lastframeMonstreStatic ;
    /// @brief tableau d'entier pour stocket le moment de la derniere image attaque affiché pour les monstres
    std::vector<unsigned int> lastframeMonstreAttack ;

    /// @brief booleen qui indique si les textures ont ete initialisé
    bool texture_partie;
    
    public :

    /// @brief constructeur par defaut de SDLMonde
    SDLMonde() {lastframe=0;}
    
    /// @brief destructeur par defaut de SDLMonde
    ~SDLMonde() {}
    
    /// @brief Procédure qui initialise toute les SDL_Texture
    void afficherInit();
    
    /// @brief Procédure qui initialise un vector d'entier par 0
    /// @param tab Tableau Dynamique vector a initialisé
    void init_vector(std::vector<unsigned int>& tab);
    
    /// @brief Procédure boucle qui fait l'affichage du monde
    void afficherBoucle();
    
    /// @brief Procédure qui intialise toute les textures pour l'animation du perso
    /// @param rectStatic Tableau de SDL_Rect qui correpond a la position de chaque image du sprite Static du perso
    /// @param rectRun Tableau de SDL_Rect qui correpond a la position de chaque image du sprite run du perso
    /// @param rectAtt Tableau de SDL_Rect qui correpond a la position de chaque image du sprite attaque du perso
    void perso_animation_init(SDL_Rect rectStatic[], SDL_Rect rectRun[], SDL_Rect rectAtt[]);
    
    /// @brief Procédure qui met a jour l'affichage des sprite du perso
    /// @param frame_actu_static entier qui correspond a l'image a afficher du Sprite static du perso
    /// @param frame_actu_run entier qui correspond a l'image a afficher du Sprite run du perso
    /// @param frame_actu_attack entier qui correspond a l'image a afficher du Sprite attaque du perso
    /// @param rectStatic Tableau de SDL_Rect qui contient toute les positions des images du sprite static du perso
    /// @param rectRun Tableau de SDL_Rect qui contient toute les positions des images du sprite run du perso
    /// @param rectAtt Tableau de SDL_Rect qui contient toute les positions des images du sprite attaque du perso
    /// @param direction entier qui correspond a la direction par rapport a l'ennemi le plus proche
    void perso_animation(int &frame_actu_static, int &frame_actu_run, int &frame_actu_attack, SDL_Rect rectStatic[], SDL_Rect rectRun[], SDL_Rect rectAtt[], int direction);
    
    /// @brief Procédure qui intialise toute les textures pour l'animation des monstres
    /// @param rectBRun Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 16 images
    /// @param rectBAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 24 images
    /// @param rectRRun Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 20 (4 images en largeur et 5 images en longueur) images
    /// @param rectRAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 30 images
    /// @param rectSAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 20 (5 images en largeur et 4 images en longueur) images
    /// @param rectBleuAtt2 Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 40 images
    void monstre_animation_init(SDL_Rect rectBRun[], SDL_Rect rectBAtt[], SDL_Rect rectRRun[], SDL_Rect rectRAtt[], SDL_Rect rectSAtt[], SDL_Rect rectBleuAtt2[]);
    
    /// @brief Procédure qui met a jour l'affichage des sprite des monstres
    /// @param frame_actu_run Tableau d'entier qui correspond a l'image a afficher du Sprite run des monstres
    /// @param rectRun Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 16 images
    /// @param frame_actu_att Tableau d'entier qui correspond a l'image a afficher du Sprite attaque des monstres
    /// @param rectAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 24 images
    /// @param rectRRun Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 20 (4 images en largeur et 5 images en longueur) images
    /// @param rectRAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 30 images
    /// @param rectSAtt Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 20 (5 images en largeur et 4 images en longueur) images
    /// @param rectBleuAtt2 Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 40 images
    void monstre_animation(std::vector<unsigned int> &frame_actu_run, SDL_Rect rectRun[], std::vector<unsigned int> &frame_actu_att, SDL_Rect rectAtt[], SDL_Rect rectRRun[], SDL_Rect rectRAtt[], SDL_Rect rectSAtt[], SDL_Rect rectBleuAtt2[]);
    
    /// @brief Procédure qui initialise l'image de la fleche pour le perso
    void arrow_animation_init();
    
    /// @brief Procédure qui met a jour l'affichage de la fleche
    void arrow_animation();
    
    /// @brief Procédure qui initialise l'image du bouclier pour le perso
    void bouclier_animation_init();
    
    /// @brief Procédure qui met a jour l'affichage du bouclier
    void bouclier_animation();
    
    /// @brief Procédure qui initialise le Sprite de l'Aura pour le perso
    /// @param rectAura Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 4 images
    void aura_animation_init(SDL_Rect rectAura[4]);
    
    /// @brief Procédure qui met a jour l'affichage du sprite de l'Aura pour le perso
    /// @param rectAura Tableau de SDL_Rect qui contient la position des images d'un Sprite avec 4 images
    /// @param frame_actu entier qui correspond a l'indice de l'image a afficher du Sprite Aura
    void aura_animation(SDL_Rect rectAura[4], int & frame_actu);
    
    /// @brief Procédure qui initialise l'image du missile pour le monstre robot
    void rocket_animation_init();
    
    /// @brief Procédure qui met a jour l'affichage du missile
    void rocket_animation();

    /// @brief Initialise les textures du terrain
    void init_terrain();

    /// @brief permet d'afficher les tilesets du terrain
    /// @param t le numéro du terrain à afficher [0,2]
    void affiche_terrain(int t);

    /// @brief Detruit toutes les textures du SDLMonde
    void afficherDetruit();

    /// @brief Permet d'afficher l'écran concerné
    void afficher();

    /// @brief Affiche le menu
    void afficherMenu();

    /// @brief Initialise les textures pour l'interface du perso
    void init_interfaceJoueur();

    /// @brief Affiche à l'ecran les stats du perso (PV, progression monde ...)
    void afficheInterfaceJoueur();
    bool niveauAChanger();

    /// @brief Affiche l'écran d'option
    void afficheOption();

    /// @brief Affiche l'ecran lorsque les PV du personnage atteignent 0
    void affichePerdu();

    /// @brief Initialise les textures des bonus
    void initBonus();

    /// @brief Permet d'afficher les bonus à l'écran lorsque le personnage atteint un certain niveau
    /// @param tabChoix Tableau qui permet de stocker les différent bonus affiché pour pouvoir appliqué celui qui est choisi
    void afficheBonus(int tabChoix[3]);

    /// @brief Permet d'appliquer un bonus à un Personnage
    /// @param b le choix du bonus
    void appliqueBonus(int b);

    /// @brief booleen qui indique si le jeu est en cours
    bool estLance;
};

#endif
