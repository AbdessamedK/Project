package Modele;

import java.util.Arrays ;

public abstract class Piece implements Runnable, Cloneable{
    private int taille;
    protected CaseSimple[] tab;
    protected int etat ; // Pour la direction de la piece pour Rotate
    
    public Piece(int x) {
        this.taille = x;
        this.tab = new CaseSimple[this.taille];
        this.etat = 0 ;
    }

    public int getTaille() {
        return this.taille;
    }
    
    public CaseSimple[] getTableauCase() {
        return Arrays.copyOf(this.tab,this.taille) ;
    }
    
    public void action(int x) {
        for (int i=0; i<this.taille; ++i) {
            tab[i].action(x, 0);
        }
    }
    
    public abstract void Rotation();
    
    @Override
    public void run() {
        for (int i=0; i<this.taille; ++i) {
            tab[i].run() ;
        }
    }
    
    @Override
    public Object clone() throws CloneNotSupportedException{
        Piece copie = (Piece) super.clone() ;
        copie.tab = Arrays.copyOf(this.tab, this.taille) ;
        for (int i=0; i<this.taille; ++i) {
            copie.tab[i] = (CaseSimple) this.tab[i].clone() ;
        }
        return copie ;
    }
}
