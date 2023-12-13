package Modele;

import java.util.Observable ;

public class GrilleSimple extends Observable {
    private final CaseSimple[][] tabCase ;
    public final int HEIGHT = 20, WIDTH = 10 ;
    private Audio audio ;

    public GrilleSimple() {
        this.tabCase = new CaseSimple[this.WIDTH][this.HEIGHT] ;
        for (int i=0; i<this.HEIGHT; ++i) {
            for (int j=0; j<this.WIDTH; ++j) {
                this.tabCase[j][i] = new CaseSimple(j,i,0,false) ;
            }
        }
        this.audio = new Audio() ;
        setChanged() ;
        notifyObservers() ;
    }
    
    public CaseSimple[][] getTabCase() {
        return this.tabCase ;
    }
    
    private boolean verifLigne(int ligne) {
        boolean occupe = true ;
        for (int i=0; i<this.WIDTH; ++i) {
            occupe = occupe && this.tabCase[i][ligne].getOccupe() ;
        }
        return occupe ;
    }
    
    private void supprimeLigne(int ligne) {
        for (int i=ligne; i>=1; --i) {
            for (int j=0; j<this.WIDTH; ++j) {
                this.tabCase[j][i] = this.tabCase[j][i-1] ;
                this.tabCase[j][i].setXY(j, i);
            }
        }
        for (int i=0; i<this.WIDTH; ++i) {
            this.tabCase[i][0] = new CaseSimple(i,0,0,false) ;
        }
    }
    
    // retourne le nombre de ligne supprimer pour la gestion du score
    public int update() {
        int j = 0 ;
        for (int i=0; i<this.HEIGHT; ++i) {
            if (verifLigne(i)) {
                supprimeLigne(i) ;
                ++j ;
                this.audio.playAudioLine();
            }
        }
        setChanged() ;
        notifyObservers() ;
        return j ;
    }
       
    public boolean validePosition(int nextX, int nextY) {
        return ((nextX >= 0) && (nextX < this.WIDTH) && (nextY >= 0) && (nextY < this.HEIGHT)
                && !(this.tabCase[nextX][nextY].getOccupe())) ;
    }
    
    public void inserPiece(Piece p) {
        CaseSimple[] tabcase = p.getTableauCase() ;
        int taille = p.getTaille() ;
        CaseSimple c ;
        
        for (int i=0; i<taille; ++i) {
            c = tabcase[i] ;
            this.tabCase[c.getx()][c.gety()] = new CaseSimple(c.getx(),c.gety(),c.getCodeCouleur(),true) ;
        }
        setChanged() ;
        notifyObservers() ;
    }
}