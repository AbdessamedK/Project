package fr.univ_lyon1.info.m1.microblog.controller;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.User;
import fr.univ_lyon1.info.m1.microblog.model.Y;
import fr.univ_lyon1.info.m1.microblog.view.ThemeStrategy;

/**
 * Controller class that manages interactions between the view and the model.
 */ 
public class Controller {
    private final Y model;

    /** Initializes the controller with the model passed as a parameter. */
    public Controller(final Y y) {
        this.model = y;
    }

    /** Add a message to the model. */
    public void addMessage(final Message message) {
        this.model.add(message);
    }

    /** Delete a message to the model. */
    public void deleteMessage(final Message message) {
        this.model.delete(message);
    }

    /** Set the bookmarked metadata of a message according to a user. */
    public void setBookmarked(final Message m, final User u, final boolean bookmarked) {
        this.model.setBookmarked(m, u, bookmarked);
    }

    /** Set the strategy to calculate the score. */
    public void setStrategySelected(final ScoreStrategy s, final User user) {
        model.setScoringStrategy(s, user);
    }

    /** Set the strategy to choose the theme. */
    public void setThemeSelected(final ThemeStrategy s, final User user) {
        String theme = s.getClass().getSimpleName();
        model.setThemeStrategy(theme, user);
    }
}
