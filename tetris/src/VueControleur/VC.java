package VueControleur;

import Modele.Audio;
import Modele.JeuMulti;
import Modele.JeuTetris;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

public class VC extends JFrame implements Observer {

    JTextField jt = new JTextField("");
    JeuTetris jeu ;
    JeuMulti jeuMulti;

    Audio audio;

    Observer screenMenu;
    Observer screenGame;
    Observer screenGameMulti;
    Observer screenPause;
    Observer screenPauseJ1;
    Observer screenPauseJ2;
    Observer screenGameOver;
    Observer screenFiniMult;


    private int ecranAct;
    private boolean jeuSimple, audio_m_lancer, audio_g_lancer ;
    private CardLayout cardLayout;
    private JPanel cards;

    private Executor ex =  Executors.newSingleThreadExecutor();

    public VC() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);

        createJeuSimple();
        jeuSimple = true;
        audio = new Audio();
        audio.playAudioMenu();
        audio_m_lancer = true ;
        audio_g_lancer = false ;

        setSize(960, 540);
        cardLayout = new CardLayout();
        cards = new JPanel(cardLayout);

        screenMenu = new ScreenMenu();
        screenPause = new ScreenPause();
        screenGameOver = new ScreenGameOver();

        cards.add((Component) screenMenu, "Menu");
        cards.add((Component) screenPause, "Pause");
        cards.add((Component) screenGameOver, "Over");


        add(cards);
        ecranAct = 0;
        cardLayout.show(cards, "Menu");


        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        manager.addKeyEventDispatcher(new KeyEventDispatcher() {
            @Override
            public boolean dispatchKeyEvent(KeyEvent keyEvent) {
                if (keyEvent.getID()==KeyEvent.KEY_PRESSED) {
                    switch(ecranAct) {
                        case 0:
                            switch (keyEvent.getKeyCode()) {
                                case KeyEvent.VK_ENTER:
                                    jeuSimple = true;
                                    ecranAct = 1;
                                    jeu.reprendre();
                                    screenGame = new ScreenGame(jeu);
                                    cards.add((Component) screenGame, "Game");
                                    cardLayout.show(cards, "Game");
                                    break;
                                case KeyEvent.VK_M:
                                    jeuSimple = false;
                                    ecranAct = 4;
                                    createJeuMulti();
                                    jeuMulti.reprendrej1();
                                    jeuMulti.reprendrej2();
                                    screenGameMulti = new ScreenGameMulti(jeuMulti);
                                    screenPauseJ1 = new ScreenPauseJ1(jeuMulti.getJoueur2());
                                    screenPauseJ2 = new ScreenPauseJ2(jeuMulti.getJoueur1());
                                    cards.add((Component) screenGameMulti, "GameMulti");
                                    cards.add((Component) screenPauseJ1, "PauseJ1");
                                    cards.add((Component) screenPauseJ2, "PauseJ2");
                                    cardLayout.show(cards, "GameMulti");
                                    break;
                                case KeyEvent.VK_R:
                                    if(jeuSimple)
                                    {
                                        System.out.println("Dans R");
                                        jeu.resetScore();
                                    }
                                     break;
                                case KeyEvent.VK_Q:
                                    closeApp();
                                    break;
                            }
                            break;
                        case 1:
                                switch (keyEvent.getKeyCode()) {
                                    case KeyEvent.VK_LEFT:
                                        jeu.action(-1);
                                        break;
                                    case KeyEvent.VK_RIGHT:
                                        jeu.action(1);
                                        break;
                                    case KeyEvent.VK_UP:
                                        try {
                                            jeu.RotatePiece();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break;
                                    case KeyEvent.VK_DOWN:
                                        jeu.DescendrePiece();
                                        break;
                                    case KeyEvent.VK_P:
                                        ecranAct = 3;
                                        jeu.pause();
                                        cardLayout.show(cards,"Pause");
                                        break;
                                    case KeyEvent.VK_SPACE:
                                    {
                                        try {
                                            jeu.HardDrop();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break ;
                                    }
                                }

                            break;

                        case 2:
                            switch (keyEvent.getKeyCode()) {
                                case KeyEvent.VK_ENTER:
                                    ecranAct = 1;
                                    createJeuSimple();
                                    screenGame = new ScreenGame(jeu);
                                    jeu.reprendre();
                                    cards.add((Component) screenGame, "Game");
                                    cardLayout.show(cards, "Game");
                                    break;
                                case KeyEvent.VK_Q:
                                    closeApp();
                                    break;
                            }
                            break;
                        case 3 :
                            switch (keyEvent.getKeyCode()) {
                                case KeyEvent.VK_ENTER:
                                    if (jeuSimple)
                                    {
                                        ecranAct = 1;
                                        jeu.reprendre();
                                        cardLayout.show(cards, "Game");
                                    }
                                    else
                                    {
                                        ecranAct = 4;
                                        jeuMulti.reprendrej1();
                                        jeuMulti.reprendrej2();
                                        cardLayout.show(cards, "GameMulti");
                                    }
                                    break;

                                case KeyEvent.VK_R:
                                    if(jeuSimple)
                                    {
                                        ecranAct = 1;
                                        createJeuSimple();
                                        screenGame = new ScreenGame(jeu);
                                        jeu.reprendre();
                                        cards.add((Component) screenGame, "Game");
                                        cardLayout.show(cards, "Game");
                                    }
                                    else
                                    {
                                        ecranAct = 4;
                                        createJeuMulti();
                                        jeuMulti.reprendrej1();
                                        jeuMulti.reprendrej2();
                                        screenGameMulti = new ScreenGameMulti(jeuMulti);
                                        screenPauseJ1 = new ScreenPauseJ1(jeuMulti.getJoueur2());
                                        screenPauseJ2 = new ScreenPauseJ2(jeuMulti.getJoueur1());
                                        cards.add((Component) screenGameMulti, "GameMulti");
                                        cards.add((Component) screenPauseJ1, "PauseJ1");
                                        cards.add((Component) screenPauseJ2, "PauseJ2");
                                        cardLayout.show(cards, "GameMulti");
                                    }
                                    break;
                                case KeyEvent.VK_Q:
                                    closeApp();
                                    break;
                            }
                            break;
                        case 4:
                            switch (keyEvent.getKeyCode()) {

                                // Action joueur droit
                                case KeyEvent.VK_LEFT:
                                    jeuMulti.actionj2(-1);
                                    break;
                                case KeyEvent.VK_RIGHT:
                                    jeuMulti.actionj2(1);
                                    break;
                                case KeyEvent.VK_UP:
                                    try {
                                        jeuMulti.Rotatej2();
                                    } catch (CloneNotSupportedException ex) {
                                        Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                    }
                                    break;
                                case KeyEvent.VK_DOWN:
                                    jeuMulti.Descendrej2();
                                    break;
                                case KeyEvent.VK_SPACE: {
                                    try {
                                        jeuMulti.HardDropj2();
                                    } catch (CloneNotSupportedException ex) {
                                        Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                    }
                                    break;
                                }

                                case KeyEvent.VK_P:
                                    ecranAct = 5;
                                    jeuMulti.pausej2();
                                    cardLayout.show(cards, "PauseJ2");
                                    break;

                                case KeyEvent.VK_K:
                                    closeApp();
                                    break;


                                // Action joueur gauche
                                case KeyEvent.VK_Q:
                                    jeuMulti.actionj1(-1);
                                    break;
                                case KeyEvent.VK_D:
                                    jeuMulti.actionj1(1);
                                    break;
                                case KeyEvent.VK_Z:
                                    try {
                                        jeuMulti.Rotatej1();
                                    } catch (CloneNotSupportedException ex) {
                                        Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                    }
                                    break;
                                case KeyEvent.VK_S:
                                    jeuMulti.Descendrej1();
                                    break;
                                case KeyEvent.VK_A: {
                                    try {
                                        jeuMulti.HardDropj1();
                                    } catch (CloneNotSupportedException ex) {
                                        Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                    }
                                    break;
                                }

                                case KeyEvent.VK_R:
                                    ecranAct = 6;
                                    jeuMulti.pausej1();
                                    cardLayout.show(cards, "PauseJ1");
                                    break;

                            }
                            break;


                            case 5 :
                                switch (keyEvent.getKeyCode()) {
                                    case KeyEvent.VK_P:
                                        ecranAct = 4;
                                        jeuMulti.reprendrej2();
                                        cardLayout.show(cards, "GameMulti");
                                        break;

                                    case KeyEvent.VK_R:
                                        ecranAct = 3;
                                        jeuMulti.pausej1();
                                        cardLayout.show(cards, "Pause");
                                        break;

                                    // Action joueur gauche
                                    case KeyEvent.VK_Q :
                                        jeuMulti.actionj1(-1);
                                        break;
                                    case KeyEvent.VK_D:
                                        jeuMulti.actionj1(1);
                                        break;
                                    case KeyEvent.VK_Z:
                                        try {
                                            jeuMulti.Rotatej1();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break;
                                    case KeyEvent.VK_S:
                                        jeuMulti.Descendrej1();
                                        break;
                                    case KeyEvent.VK_A:
                                    {
                                        try {
                                            jeuMulti.HardDropj1();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break ;
                                    }
                                }
                                break;

                            case 6 :
                                switch (keyEvent.getKeyCode()) {

                                    case KeyEvent.VK_R:
                                        ecranAct = 4;
                                        jeuMulti.reprendrej1();
                                        cardLayout.show(cards, "GameMulti");
                                        break;

                                    case KeyEvent.VK_P:
                                        ecranAct = 3;
                                        jeuMulti.pausej1();
                                        cardLayout.show(cards, "Pause");
                                        break;

                                    // Action joueur droit
                                    case KeyEvent.VK_LEFT:
                                        jeuMulti.actionj2(-1);
                                        break;
                                    case KeyEvent.VK_RIGHT:
                                        jeuMulti.actionj2(1);
                                        break;
                                    case KeyEvent.VK_UP:
                                        try {
                                            jeuMulti.Rotatej2();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break;
                                    case KeyEvent.VK_DOWN:
                                        jeuMulti.Descendrej2();
                                        break;
                                    case KeyEvent.VK_SPACE: {
                                        try {
                                            jeuMulti.HardDropj2();
                                        } catch (CloneNotSupportedException ex) {
                                            Logger.getLogger(VC.class.getName()).log(Level.SEVERE, null, ex);
                                        }
                                        break;
                                    }
                                }
                                break;

                        case 7:
                            switch (keyEvent.getKeyCode()) {
                                case KeyEvent.VK_R:
                                    ecranAct = 4;
                                    createJeuMulti();
                                    jeuMulti.reprendrej1();
                                    jeuMulti.reprendrej2();
                                    screenGameMulti = new ScreenGameMulti(jeuMulti);
                                    screenPauseJ1 = new ScreenPauseJ1(jeuMulti.getJoueur2());
                                    screenPauseJ2 = new ScreenPauseJ2(jeuMulti.getJoueur1());
                                    cards.add((Component) screenGameMulti, "GameMulti");
                                    cards.add((Component) screenPauseJ1, "PauseJ1");
                                    cards.add((Component) screenPauseJ2, "PauseJ2");
                                    cardLayout.show(cards, "GameMulti");
                                    break;
                                case KeyEvent.VK_Q:
                                    closeApp();
                                    break;

                            }
                    }
                    switch(ecranAct) {
                        case 0,2,3 :
                            if (audio_g_lancer) {
                               audio.stopAudioGame(); 
                               audio_g_lancer = false ;
                            }
                            if (audio_m_lancer) {
                                if (audio.isMenuAudioEnd()) {
                                    audio.playAudioMenu();
                                }
                            }
                            else {
                                audio.playAudioMenu();
                                audio_m_lancer = true ;
                            }
                            break ;
                        case 1,4,5,6 :
                            if (audio_m_lancer) {
                                audio.stopAudioMenu();
                                audio_m_lancer = false ;
                            }
                            if (audio_g_lancer) {
                                if (audio.isGameAudioEnd()) {
                                    audio.playAudioGame();
                                }
                            }
                            else {
                                audio.playAudioGame();
                                audio_g_lancer = true ;
                            }
                            break ;
                    }
                }
                return true;
            }
        });
    }


    private void createJeuSimple()
    {
        this.jeu = new JeuTetris();
        this.jeu.addObserver(this);
    }

    private void createJeuMulti()
    {
        this.jeuMulti = new JeuMulti(this);
    }


    private void closeApp() {
        dispose(); // Ferme la fenêtre
        System.exit(0); // Arrêt complet du programme
    }
    
    @Override
    public void update(Observable o, Object arg) { // rafraichissement de la vue
        SwingUtilities.invokeLater(new Runnable() {
            //@Override
            public void run() {
                if(jeuSimple)
                {
                    if (jeu.estFini())
                    {
                        ecranAct = 2;
                        cardLayout.show(cards, "Over");
                    }
                    screenGame.update(o, arg);
                }
                else
                {
                    if(jeuMulti.estFini()[0])
                    {
                        ecranAct = 7;
                        screenFiniMult = new ScreenFiniMult(2);
                        cards.add((Component) screenFiniMult, "FiniMult");
                        cardLayout.show(cards, "FiniMult");
                    }
                    if(jeuMulti.estFini()[1])
                    {
                        ecranAct = 7;
                        screenFiniMult = new ScreenFiniMult(1);
                        cards.add((Component) screenFiniMult, "FiniMult");
                        cardLayout.show(cards, "FiniMult");
                    }
                    screenPauseJ1.update(o,arg);
                    screenPauseJ2.update(o,arg);
                    screenGameMulti.update(o, arg);
                }
            }
        });


    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    VC vc = new VC();
                    vc.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                    vc.setVisible(true);
                }
            }
        );
    }
}
