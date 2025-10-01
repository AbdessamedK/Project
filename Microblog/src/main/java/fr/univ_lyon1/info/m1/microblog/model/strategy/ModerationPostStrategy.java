package fr.univ_lyon1.info.m1.microblog.model.strategy;


import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.util.Arrays;
import java.util.List;
import java.util.Set;

/**
 * Implements a scoring strategy for messages based on moderation criteria.
 * This strategy calculates a score for a given message by penalizing it
 * for the presence of forbidden words.
 * <p>
 * The following words are considered forbidden:
 * <ul>
 *     <li>spam</li>
 *     <li>fake</li>
 *     <li>fck</li>
 * </ul>
 * Each occurrence of a forbidden word results in a penalty of 50 points.
 * </p>
 */
public final class ModerationPostStrategy implements ScoreStrategy {
    private static ModerationPostStrategy instance;

    private ModerationPostStrategy() { }

    /** Function that returns only one instance of the class. */
    public static ModerationPostStrategy getInstance() {
        if (instance == null) {
            instance = new ModerationPostStrategy();
        }
        return instance;
    }
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        List<String> forbiddenWords = Arrays.asList("spam", "fake", "fck");
        int malus = 0;
        for (String w: forbiddenWords) {
            if (m.getContent().contains(w)) {
                malus -= 50;
            }
        }
        return malus;

    }

    @Override
    public int compare(final Message leftMessage,
                       final MessageData leftData,
                       final Message rightMessage,
                       final MessageData rightData) {
        int scoreLeft = leftData.getScore();
        int scoreRight = rightData.getScore();


        if (leftData.isBookmarked() && !rightData.isBookmarked()) {
            return 1;
        }
        if (!leftData.isBookmarked() && rightData.isBookmarked()) {
            return -1;
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
