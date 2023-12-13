package Modele;

public class Tool {
    public static int monRandom(int min, int max) {
        return (int) (Math.random() * (max-min + 1)) + min ;
    }
}