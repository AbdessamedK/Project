package fr.univ_lyon1.info.m1.microblog.model;

import java.util.HashSet;
import java.util.Set;

import fr.univ_lyon1.info.m1.microblog.model.strategy.ChronologicalPostStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import javafx.util.Pair;

/**
 * Piece of data associated to a message for a particular user.
 */
public class MessageData {
    /**
     * Represents the data associated with a message.
     */
    private boolean isBookmarked = false;
    private int score = -1;
    private User owner;
    private Set<String> words = new HashSet<>();
    
    private final Pair<User, Message> id;

    /**
     * Build a MessageData object from it's User and Message.
     */
    public MessageData(final User user, final Message message) {
        this.id = new Pair<>(user, message);
    }

    public Pair<User, Message> getId() {
        return this.id;
    }

    public User getOwner() {
        return this.owner;
    }

    public void setOwner(final User user) {
        this.owner = user;
    }

    /**
     * Returns the set of words associated with the message.
     *
     * @return the set of words
     */
    public Set<String> getWords() {
        return words;
    }

    /**
     * Sets the set of words associated with the message.
     *
     * @param words the set of words
     */
    public void setWords(final Set<String> words) {
        this.words = words;
    }

    /**
     * Returns the score of the message.
     *
     * @return the score
     */
    public int getScore() {
        return score;
    }

    /**
     * Sets the score of the message.
     *
     * @param score the score
     */
    public void setScore(final int score) {
        this.score = score;
    }

    /**
     * Returns whether the message is bookmarked or not.
     *
     * @return true if the message is bookmarked, false otherwise
     */
    public boolean isBookmarked() {
        return isBookmarked;
    }

    /**
     * Sets whether the message is bookmarked or not.
     *
     * @param bookmarked true if the message is bookmarked, false otherwise
     */
    public void setBookmarked(final boolean bookmarked) {
        this.isBookmarked = bookmarked;
    }

    /**
     * Compare two Messages metadata. Suitable for sorting.
     */
    public int compare(final MessageData rightData, final ScoreStrategy s) {
        int scoreLeft = getScore();
        int scoreRight = rightData.getScore();

        if (!(s instanceof ChronologicalPostStrategy)) {
            if (isBookmarked() && !rightData.isBookmarked()) {
                return 1;
            }
            if (!isBookmarked() && rightData.isBookmarked()) {
                return -1;
            }
        }

        if (scoreLeft < scoreRight) {
            return -1;
        } else if (scoreLeft == scoreRight) {
            return 0;
        } else if (scoreLeft > scoreRight) {
            return 1;
        }
        throw new AssertionError("Case not covered in comparision");
    }
}
