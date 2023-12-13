package Modele;

import java.util.Observable;
import java.util.List;

public class JeuTetris extends Observable implements Runnable {
    private GrilleSimple grille ;
    private Piece[] tabp ;
    private OrdonnanceurSimple ordonnanceur ;
    private int idActif, difficulty ;
    private Score score ;
    private boolean est_fini, a_ecrit_score ;
    
    public JeuTetris() {
        this.grille = new GrilleSimple();
        this.tabp = new Piece[2];

        this.tabp[0] = GenererPiece();
        this.tabp[1] = GenererPiece();

        this.idActif = 0;
        this.difficulty = 1 ;
        this.score = new Score() ;
        this.est_fini = false ;
        this.a_ecrit_score = false ;

        // Appel à votre méthode update
        this.ordonnanceur = new OrdonnanceurSimple(this);

        this.ordonnanceur.start();
        this.ordonnanceur.mettreEnPause();
    }
        
    public GrilleSimple getGrille() {
        return this.grille ;
    }
    
    public Piece getPieceActif() {
        try {
            return (Piece) this.tabp[this.idActif].clone() ;
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        return null ;
    }
    
    public Piece getPieceSuivante() {
        try {
            return (Piece) this.tabp[(this.idActif+1)%2].clone() ;
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        return null ;
    }
    
    private Piece GenererPiece() {
        int rand = Tool.monRandom(1,7) ;
        switch (rand) {
            case 1 : return new PieceCarre() ;
            case 2 : return new PieceBaton() ;
            case 3 : return new PieceJ() ;
            case 4 : return new PieceL() ;
            case 5 : return new PieceS() ;
            case 6 : return new PieceTe() ;
            case 7 : return new PieceZ() ;
        }
        return new PieceCarre() ;
    }
    
    private boolean peutDescendre(Piece p) {
        CaseSimple[] tabCase = p.getTableauCase() ;
        boolean peutDes = true ;
        for (int i=0; i<p.getTaille(); ++i) {
            if (!grille.validePosition(tabCase[i].getx(), tabCase[i].gety()+tabCase[i].getdy()))
                peutDes = false ;
        }
        return peutDes ;
    }
    
    private int ScoreToAdd(int nbligne) {
        return switch (nbligne) {
            case 0 -> 0;
            case 1 -> 100 * this.difficulty;
            case 2 -> 300 * this.difficulty;
            case 3 -> 500 * this.difficulty;
            default -> 800 * this.difficulty;
        };
    }
    
    public boolean estFini() {
        return this.est_fini ;
    }
     
    public int getDifficulty() {
        return this.difficulty ;
    }
    
    private void update_difficuly() {
        int s = this.score.getScore() ;
        if (s>1000 && this.difficulty==1) {
            ++this.difficulty ;
            ordonnanceur.setSpeed(ordonnanceur.getSpeed()-50);
        }
        else if (s>2000 && this.difficulty==2) {
            ++this.difficulty ;
            ordonnanceur.setSpeed(ordonnanceur.getSpeed()-50);
        } 
        else if (s>5000 && this.difficulty==3) {
            ++this.difficulty ;
            ordonnanceur.setSpeed(ordonnanceur.getSpeed()-50);
        }
        else if (s>10000 && this.difficulty==4) {
            ++this.difficulty ;
            ordonnanceur.setSpeed(ordonnanceur.getSpeed()-50);
        } 
        else if (s>20000 && this.difficulty==5) {
            ++this.difficulty ;
            ordonnanceur.setSpeed(ordonnanceur.getSpeed()-100);
        }
    }
    
    private void update() {
        if (peutDescendre(tabp[idActif])) {
            tabp[idActif].run();
        }
        else {
            grille.inserPiece(tabp[idActif]);
            this.score.addScore(12);
            int nbligne = grille.update();
            this.score.addScore(ScoreToAdd(nbligne));
            tabp[idActif] = GenererPiece() ;
            idActif = (idActif + 1) % 2 ;
            this.est_fini = !(peutDescendre(tabp[idActif])) ;
        }
        update_difficuly() ;
    }
    
    public List<Integer> getHighScore() {
        return this.score.readScore((getClass().getResource("../data/score.txt")).getPath()) ;
    }
    
    public int getScore() {
        return this.score.getScore();
    }
    
    public void resetScore() {
        this.score.resetScore((getClass().getResource("../data/score.txt")).getPath());
    }
    
    public void DescendrePiece() {        
        if (peutDescendre(tabp[idActif])) {
            tabp[idActif].run();
            setChanged();
            notifyObservers();
        }
    }
    
    public void HardDrop() throws CloneNotSupportedException {
        Piece tempPiece = (Piece) tabp[idActif].clone() ;
        while (peutDescendre(tempPiece)) {
            tempPiece.run();
        }
        CaseSimple[] tempCase = tempPiece.getTableauCase() ;
        for (int i=0; i<tempPiece.getTaille(); ++i) {
            tabp[idActif].getTableauCase()[i].setXY(tempCase[i].getx(),tempCase[i].gety());
        }
        
        setChanged();
        notifyObservers();
    }
    
    public void RotatePiece() throws CloneNotSupportedException {
        Piece tempPiece = (Piece) tabp[idActif].clone() ;
        boolean peutRotate = true ;
        
        tempPiece.Rotation();
        CaseSimple[] tabtemp = tempPiece.getTableauCase();
        
        for (int i=0; i<tempPiece.getTaille(); ++i) {
            if (!grille.validePosition(tabtemp[i].getx(), tabtemp[i].gety()))
                peutRotate = false ;
        }

        if (peutRotate) {
            tabp[idActif].Rotation();
            setChanged();
            notifyObservers();
            
        }
    }
    
    public void action(int x) {
        CaseSimple[] tabCase = tabp[idActif].getTableauCase() ;
        boolean peutTranslater = true ;
        for (int i=0; i<tabp[idActif].getTaille(); ++i) {
            if (!grille.validePosition(tabCase[i].getx()+x,tabCase[i].gety()))
                peutTranslater = false ;
        }
        
        if (peutTranslater) {
            tabp[idActif].action(x);
            setChanged();
            notifyObservers();
        }
    }
    
    @Override
    public void run() {
        if (!this.est_fini) {
            update() ;
        }
        else if (!this.a_ecrit_score) {
            this.score.writeScore((getClass().getResource("../data/score.txt")).getPath());
            this.a_ecrit_score = true ;
        }
        setChanged();
        notifyObservers();
    }

    public int getWidth() {
        return grille.WIDTH ;
    }
    
    public int getHeight() {
        return grille.HEIGHT ;
    }
    
    public void reprendre()
    {
        this.ordonnanceur.reprendre();
    }
    
    public void pause()
    {
        this.ordonnanceur.mettreEnPause();
    }
}
