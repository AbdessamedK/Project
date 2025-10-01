package fr.univ_lyon1.info.m1.microblog.view;

import javafx.scene.layout.VBox;

/** Class to applicate the Dark Theme. */
public final class DarkTheme implements ThemeStrategy {
    private static DarkTheme instance;

    private DarkTheme() { }

    /** Function that returns only one instance of the class. */
    public static DarkTheme getInstance() {
        if (instance == null) {
            instance = new DarkTheme();
        }
        return instance;
    }
    /**
     * Apply the Normal Theme.
     */
    @Override
    public void applyTheme(final VBox v) {
        v.getStyleClass().add("dark-mode");
        v.getStyleClass().remove("light-mode");
    }
    
      @Override
    public String toString() {
        return "Mode Nocturne";
    }
}
