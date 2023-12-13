package VueControleur;

import Modele.CaseSimple;
import Modele.GrilleSimple;
import Modele.JeuMulti;
import Modele.JeuTetris;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Objects;
import java.util.Observable;

public class ScreenGameMulti extends Screen{



    // 352 11 24
    private double DEBUTX;
    private double DEBUTY;
    private  double TAILLE = 24.8 ;
    private JeuMulti jeu;
    private Image backgroundImage; // Image de fond

    public ScreenGameMulti(JeuMulti _jeu)
    {
        this.jeu = _jeu;
        setLayout(new BorderLayout());
        Dimension dim = new Dimension((int) DEBUTX,(int) DEBUTY);
        setPreferredSize(dim);
        initialize();
    }


    public void initialize()
    {
        // Définition de l'image de fond
        ImageIcon backgroundImageIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("../data/5.png")));
        System.out.println(this.getWidth()) ;
        backgroundImage = backgroundImageIcon.getImage();

    }

    // Méthode pour définir l'image de fond

    private Color CalculCouleur(int CodeCouleur) {
        switch (CodeCouleur) {
            case 0 : return Color.WHITE ;
            case 1 : return Color.CYAN ;
            case 2 : return Color.YELLOW ;
            case 3 : return Color.MAGENTA ;
            case 4 : return Color.ORANGE ;
            case 5 : return Color.BLUE ;
            case 6 : return Color.RED ;
            case 7 : return Color.GREEN ;
        }
        return Color.BLACK ;
    }


    @Override
    protected void paintComponent(Graphics g) {
        //System.out.println(DEBUT);
        super.paintComponent(g);
        if (backgroundImage != null)
        {
            g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
            DEBUTX = (118.0 * this.getWidth()) / 1920.0;
            DEBUTY = (23.0 * this.getHeight()) / 1080.0;

            JeuTetris j1 = jeu.getJoueur1();
            JeuTetris j2 = jeu.getJoueur2();

            afficheGrille(g, DEBUTX, DEBUTY, j1);


            DEBUTX = (1325.0 * this.getWidth()) / 1920.0;
            DEBUTY = (23.0 * this.getHeight()) / 1080.0;

            afficheGrille(g, DEBUTX, DEBUTY, j2);

            DEBUTX = (794.0 * this.getWidth()) / 1920.0;
            DEBUTY = (400.0 * this.getHeight()) / 1080.0;

            affichePieceSuivante(g, DEBUTX, DEBUTY, j1);

            DEBUTX = (1080.0 * this.getWidth()) / 1920.0;
            DEBUTY = (400.0 * this.getHeight()) / 1080.0;

            affichePieceSuivante(g, DEBUTX, DEBUTY, j2);

            DEBUTX = (740 * this.getWidth()) / 1920.0;
            DEBUTY = (820 * this.getHeight()) / 1080.0;

            dessinerTexte(g, Color.white, j1.getScore(), (int) DEBUTX, (int) DEBUTY, 50);

            DEBUTX = (1080 * this.getWidth()) / 1920.0;

            dessinerTexte(g, Color.white, j2.getScore(), (int) DEBUTX, (int) DEBUTY, 50);





        }

    }

    private void affichePieceSuivante(Graphics g , double x, double y, JeuTetris _jeu)
    {
        int _TAILLE = 16;
        CaseSimple[] tabC1 = _jeu.getPieceSuivante().getTableauCase();
        for (int i = 0; i < _jeu.getPieceSuivante().getTaille(); ++i) {
            g.setColor(CalculCouleur(tabC1[i].getCodeCouleur()));
            g.fillRect((int) (x + ((tabC1[i].getx() - tabC1[0].getx()) * _TAILLE)), (int) (y + tabC1[i].gety() * _TAILLE), (int) _TAILLE, (int) _TAILLE);
            g.setColor(Color.BLACK);
            g.drawRoundRect((int) (x + ((tabC1[i].getx() - tabC1[0].getx()) * _TAILLE)), (int) (y + tabC1[i].gety() * _TAILLE), (int) _TAILLE, (int) _TAILLE, 1, 1);
        }
    }

    private void afficheGrille(Graphics g , double x, double y, JeuTetris _jeu )
    {

        CaseSimple[][] _grille = _jeu.getGrille().getTabCase();
        // Affiche les pièces de la grille
        for (int i=0; i<_jeu.getWidth(); ++i) {
            for (int j=0; j<_jeu.getHeight(); ++j) {
                if(_grille[i][j].getCodeCouleur() != 0)
                {
                    g.setColor(CalculCouleur(_grille[i][j].getCodeCouleur()));
                    g.fillRect((int) (x+_grille[i][j].getx()*TAILLE), (int) (y+_grille[i][j].gety()*TAILLE), (int) TAILLE, (int) TAILLE);

                    g.setColor(Color.black);
                    g.drawRoundRect((int) (x+_grille[i][j].getx()*TAILLE), (int) (y+_grille[i][j].gety()*TAILLE), (int) TAILLE, (int) TAILLE,1,1);
                }

            }
        }
        // Affiche de la pièce courante
            CaseSimple[] tabC = _jeu.getPieceActif().getTableauCase();
            for (int i = 0; i < _jeu.getPieceActif().getTaille(); ++i) {
            g.setColor(CalculCouleur(tabC[i].getCodeCouleur()));
            g.fillRect((int) (x+ tabC[i].getx() * TAILLE), (int) (y+tabC[i].gety() * TAILLE), (int) TAILLE, (int) TAILLE);
            g.setColor(Color.BLACK);
            g.drawRoundRect((int) (x+tabC[i].getx() * TAILLE), (int) (y+tabC[i].gety() * TAILLE), (int) TAILLE, (int) TAILLE, 1, 1);

        }

    }

    private void dessinerTexte(Graphics g,Color couleur, int score, int x, int y, int taillePo) {
        g.setColor(couleur);
        Font font = new Font(g.getFont().getFontName(), Font.PLAIN, taillePo);
        g.setFont(font);
        g.drawString(String.valueOf(score), x, y);
    }

    @Override
    public void update(Observable o, Object arg) {
        repaint(); // Redessine le panneau après une mise à jour
        Toolkit.getDefaultToolkit().sync(); // forcer la synchronisation
    }
}