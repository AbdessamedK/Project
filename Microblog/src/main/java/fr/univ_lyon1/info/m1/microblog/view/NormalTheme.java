package fr.univ_lyon1.info.m1.microblog.view;

import javafx.scene.layout.VBox;

/**
* Class to applicate the Normal Theme.
*/
public final class NormalTheme implements ThemeStrategy {
    private static NormalTheme instance;

    private NormalTheme() { }

    /** Function that returns only one instance of the class. */
    public static NormalTheme getInstance() {
        if (instance == null) {
            instance = new NormalTheme();
        }
        return instance;
    }
    /**
     * Apply the Normal Theme.
     */
    @Override
    public void applyTheme(final VBox v) {
        v.getStyleClass().remove("dark-mode");
        v.getStyleClass().add("light-mode");
    }
    /**
     * Name of the Theme.
     */
    @Override
    public String toString() {
        return "Mode Normal";
    }
    
}
