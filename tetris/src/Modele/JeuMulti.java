package Modele;

import java.util.Observer;

public class JeuMulti {
    private JeuTetris j1, j2 ;
    
    public JeuMulti(Observer o) {
        j1 = new JeuTetris() ;
        j2 = new JeuTetris() ;
        j1.addObserver(o);
        j2.addObserver(o);
    }

    public JeuTetris getJoueur1() { return j1;}
    public JeuTetris getJoueur2() { return j2;}

    public boolean[] estFini() {
        return new boolean[]{j1.estFini(), j2.estFini()} ;
    }
    

    
    public void actionj1(int x) {
        j1.action(x) ;
    }
    
    public void actionj2(int x) {
        j2.action(x) ;
    }
    
    public void Rotatej1() throws CloneNotSupportedException {
        j1.RotatePiece();
    }
    
    public void Rotatej2() throws CloneNotSupportedException {
        j2.RotatePiece();
    }
    
    public void Descendrej1() {
        j1.DescendrePiece();
    }
    
    public void Descendrej2() {
        j2.DescendrePiece();
    }
    
    public void HardDropj1() throws CloneNotSupportedException {
        j1.HardDrop();
    }
    
    public void HardDropj2() throws CloneNotSupportedException {
        j2.HardDrop();
    }
    
    public void reprendrej1() {
        j1.reprendre();
    }
    
    public void reprendrej2() {
        j2.reprendre();
    }
    
    public void pausej1() {
        j1.pause();
    }
    
    public void pausej2() {
        j2.pause();
    }
    
    public int getWidth() {
        return j1.getWidth();
    }
    
    public int getHeight() {
        return j1.getHeight();
    }
    
    
}
