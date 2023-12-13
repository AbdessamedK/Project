package VueControleur;

import Modele.JeuTetris;
import Modele.GrilleSimple;
import Modele.CaseSimple;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Toolkit;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JPanel;

public class VueTetris extends JPanel implements Observer {
    
    private final static int TAILLE = 16;
    private JeuTetris jeu;
    
    public VueTetris(JeuTetris _jeu) {
        jeu = _jeu ;
        setLayout(new BorderLayout());
        Dimension dim = new Dimension(TAILLE * jeu.getWidth(), TAILLE * jeu.getHeight());
        setPreferredSize(dim);
    }
    
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
    public void update(Observable o, Object arg) {
        repaint();
        Toolkit.getDefaultToolkit().sync(); // forcer la synchronisation
    }
    
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        
        CaseSimple[][] _grille = jeu.getGrille().getTabCase();
        /*
        for (int i=0; i<jeu.getWidth(); ++i) {
            for (int j=0; j<jeu.getHeight(); ++j) {
                g.setColor(CalculCouleur(_grille[i][j].getCodeCouleur()));
                g.fillRect(_grille[i][j].getx()*TAILLE, _grille[i][j].gety()*TAILLE, TAILLE, TAILLE);
                g.setColor(Color.BLACK);
                g.drawRoundRect(_grille[i][j].getx()*TAILLE, _grille[i][j].gety()*TAILLE, TAILLE, TAILLE,1,1);
            }
        }*/
        
        CaseSimple[] tabC = jeu.getPieceActif().getTableauCase();
        for (int i = 0; i < jeu.getPieceActif().getTaille(); ++i) {
            g.setColor(CalculCouleur(tabC[i].getCodeCouleur()));
            g.fillRect(tabC[i].getx() * TAILLE, tabC[i].gety() * TAILLE, TAILLE, TAILLE);
            g.setColor(Color.BLACK);
            g.drawRoundRect(tabC[i].getx() * TAILLE, tabC[i].gety() * TAILLE, TAILLE, TAILLE, 1, 1);
        }
    }
}
