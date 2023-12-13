package Modele;

public class PieceCarre extends Piece{
    private void creerPiece() {
        int x = 4, y = 0 ;
        this.tab[0] = new CaseSimple(x,y,2,true) ;
        this.tab[1] = new CaseSimple(x+1,y,2,true) ;
        this.tab[2] = new CaseSimple(x,y+1,2,true) ;
        this.tab[3] = new CaseSimple(x+1,y+1,2,true) ;
    }

    public PieceCarre() {
        super(4);   
        creerPiece();
    }
    
    @Override
    public void Rotation() {
        this.etat = (this.etat+1) % this.getTaille() ;
    }
}
