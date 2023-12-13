package VueControleur;

import javax.swing.*;
import java.awt.*;
import java.util.Observer;

public abstract class Screen extends JPanel implements Observer {
    private Image backgroundImage;
    public abstract void initialize();
}
