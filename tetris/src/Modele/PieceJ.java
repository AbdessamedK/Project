package Modele;

public class PieceJ extends Piece {
    private void creerPiece() {
        int x = 4, y = 0 ;
        this.tab[0] = new CaseSimple(x,y,5,true) ;
        this.tab[1] = new CaseSimple(x-1,y,5,true) ;
        this.tab[2] = new CaseSimple(x+1,y,5,true) ;
        this.tab[3] = new CaseSimple(x+1,y+1,5,true) ;
    }
    
    public PieceJ() { 
        super(4);
        creerPiece() ;
    }
    
    @Override
    public void Rotation() {
        int x = this.tab[0].getx() ;
        int y = this.tab[0].gety() ;
        
        switch(this.etat) {
            case 0 -> {
                this.tab[1].setXY(x,y-1);
                this.tab[2].setXY(x,y+1);
                this.tab[3].setXY(x-1,y+1);
            }
            case 1 -> {
                this.tab[1].setXY(x+1,y);
                this.tab[2].setXY(x-1,y);
                this.tab[3].setXY(x-1,y-1);
            }
            case 2 -> {
                this.tab[1].setXY(x,y+1);
                this.tab[2].setXY(x,y-1);
                this.tab[3].setXY(x+1,y-1);
            }
            case 3 -> {
                this.tab[1].setXY(x-1,y);
                this.tab[2].setXY(x+1,y);
                this.tab[3].setXY(x+1,y+1);
            }
        }
        this.etat = (this.etat+1) % 4 ;
    }
}
