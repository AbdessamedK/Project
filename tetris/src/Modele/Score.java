package Modele;

import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Collections;
import java.util.ArrayList;
import java.util.List;
import java.io.File;
import java.util.Scanner ;

public class Score {
    private int score ;
    private static final int MAX_SCORE_KEEP = 3 ;
    
    public Score() {
        this.score = 0 ;
    }
    
    public int getScore() {
        return this.score ;
    }
    
    public void addScore(int add) {
        this.score += add ;
    }
    
    public List<Integer> readScore(String filename) {
        List<Integer> integers = new ArrayList<>();
        try {
            File file = new File(filename);
            Scanner sc = new Scanner(file) ;
            String line = sc.nextLine() ;
            String[] tabScore = line.split(" ") ;
            
            for (int i=0; i<MAX_SCORE_KEEP; ++i) {
                int j = Integer.parseInt(tabScore[i]) ;
                integers.add(j);
            }
            sc.close();
            
        } catch (FileNotFoundException e) {
            System.out.println("Erreur lors de l'ouverture du fichier : "+filename);
            e.printStackTrace();
        }
        
        return integers ;
    }
    
    public void writeScore(String filename) {
        try {
            List<Integer> listScore = readScore(filename) ;
            listScore.add(this.score) ;
            Collections.sort(listScore,Collections.reverseOrder());
            FileWriter writer = new FileWriter(filename,false) ;
            for (int i=0; i<MAX_SCORE_KEEP; ++i) {
                writer.write(""+listScore.get(i));
                if (i!=MAX_SCORE_KEEP-1)
                    writer.write(" ");
            }
            writer.close();
        } catch (IOException e) {
            System.out.println("Erreur lors de l'ouverture du fichier : "+filename);
            e.printStackTrace();
        }
    }
    
    public void resetScore(String filename) {
        try {
            FileWriter writer = new FileWriter(filename,false) ;
            for (int i=0; i<MAX_SCORE_KEEP; ++i) {
                writer.write(""+0) ;
                if (i!=MAX_SCORE_KEEP-1)
                    writer.write(" ") ;
            }
            writer.close() ;
        } catch (IOException e) {
            System.out.println("Erreur lors de l'ouverture du fichier : "+filename);
            e.printStackTrace();
        }
    }
}
