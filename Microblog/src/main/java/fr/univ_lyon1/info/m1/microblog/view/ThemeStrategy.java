package fr.univ_lyon1.info.m1.microblog.view;

import javafx.scene.layout.VBox;
    /**
     * Class we to applicate a new css for User.
     */
public interface ThemeStrategy {
    /**
     * Take the scene and applicate a new css file.
     * 
     * @param scene the scene of my application
     */
    void applyTheme(VBox scene);
    
    /**
     * Name of the Class.
     */
    String toString();
    
}
