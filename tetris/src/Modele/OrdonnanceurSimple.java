package Modele;

public class OrdonnanceurSimple extends Thread {
    private final Object verrou = new Object();
    private boolean enPause;
    private final Runnable monRunnable;
    private int speed = 400 ;

    public OrdonnanceurSimple(Runnable _monRunnable) {
        monRunnable = _monRunnable;
    }

    public void mettreEnPause() {
        enPause = true;
    }

    public void reprendre() {
        enPause = false;
        synchronized (verrou) {
            verrou.notify();
        }
    }
    
    public int getSpeed() {
        return this.speed ;
    }
    
    public void setSpeed(int _speed) {
        this.speed = _speed ;
    }

    @Override
    public void run() {
        while (true) {
            synchronized (verrou) {
                while (enPause) {
                    System.out.println("je suis en pause !") ;
                    try {
                        verrou.wait(); // Met en pause le thread
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return;
                    }
                }
            }
            monRunnable.run();
            try {
                Thread.sleep(this.speed);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
