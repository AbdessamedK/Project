package VueControleur;

import javax.swing.*;
import java.awt.*;
import java.util.Objects;
import java.util.Observable;

public class ScreenGameOver extends Screen{

    private Image backgroundImage; // Image de fond

    public ScreenGameOver()
    {
        initialize();
    }


    public void initialize()
    {
        // Définition de l'image de fond
        ImageIcon backgroundImageIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("../data/3.png")));
        System.out.print("dedans");
        backgroundImage = backgroundImageIcon.getImage();
    }

    // Méthode pour définir l'image de fond

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        if (backgroundImage != null) {
            g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), this);

        }
    }

    @Override
    public void update(Observable o, Object arg) {
        repaint(); // Redessine le panneau après une mise à jour
        Toolkit.getDefaultToolkit().sync(); // forcer la synchronisation

    }
}
