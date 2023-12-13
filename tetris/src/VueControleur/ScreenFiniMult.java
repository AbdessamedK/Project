package VueControleur;

import javax.swing.*;
import java.awt.*;
import java.util.Objects;
import java.util.Observable;

public class ScreenFiniMult extends Screen{



    private Image backgroundImage; // Image de fond
    private int vainqueur;

    public ScreenFiniMult(int vainqueur)
    {
        this.vainqueur = vainqueur;
        initialize();
    }


    public void initialize()
    {
        // Définition de l'image de fond
        ImageIcon backgroundImageIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("../data/8.png")));
        System.out.print("dedans");
        backgroundImage = backgroundImageIcon.getImage();
    }

    // Méthode pour définir l'image de fond

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (backgroundImage != null) {
            g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);
            double x = (1340.0 * this.getWidth()) / 1920.0;
            double y = (837.0 * this.getHeight()) / 1080.0;

            dessinerTexte(g, Color.PINK, vainqueur, (int) x, (int) y,170);
        }
    }

    private void dessinerTexte(Graphics g,Color couleur, int score, int x, int y, int taillePo) {
        g.setColor(couleur);
        Font font = new Font(g.getFont().getFontName(), Font.PLAIN, taillePo);
        g.setFont(font);
        g.drawString(String.valueOf(score), x, y);
    }

    @Override
    public void update(Observable o, Object arg) {
        repaint(); // Redessine le panneau après une mise à jour
        Toolkit.getDefaultToolkit().sync(); // forcer la synchronisation

    }
}
