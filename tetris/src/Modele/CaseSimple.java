package Modele;

public class CaseSimple implements Cloneable{
    
    private int x, y, dy=1, codeCouleur ;
    private boolean occupe ;
   
    public CaseSimple(int _x, int _y, int _codeCouleur, boolean _occupe) {
        this.x = _x ;
        this.y = _y ;
        this.codeCouleur = _codeCouleur ;
        this.occupe = _occupe ;
    }

    public void action(int _x, int _y) { // decalage en x et y
        this.x += _x ;
        this.y += _y ;
    }

    public void run() {
        int nextY = y;
        int nextX = x;
        nextY += this.dy;
        this.y = nextY;
        this.x = nextX;
    }

    public int getx() {
        return x;
    }

    public int gety() {
        return y;
    }
    
    public void setXY(int _x, int _y) {
        this.x = _x ;
        this.y = _y ;
    }
    
    public int getdy() {
        return this.dy ;
    }
    
    public int getCodeCouleur() {
        return this.codeCouleur ;
    }
    
    public boolean getOccupe() {
        return this.occupe ;
    }
    
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone() ;
    }
}
