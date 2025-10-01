package fr.univ_lyon1.info.m1.microblog.model.strategy;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.util.Set;

/**
 * Implements a scoring strategy for messages based on bookmarked words.
 * This strategy calculates a score for a given message by counting
 * the occurrences of words that are bookmarked by the user.
 */
public final class BookmarkedWordStrategy  implements ScoreStrategy {
    private static BookmarkedWordStrategy instance;

    private BookmarkedWordStrategy() { }

    /** Function that returns only one instance of the class. */
    public static BookmarkedWordStrategy getInstance() {
        if (instance == null) {
            instance = new BookmarkedWordStrategy();
        }
        return instance;
    }

    /**
     * Calculates the score for a given message based on the presence of
     * bookmarked words in the message data.
     *
     * @param m the message to be scored
     * @param bookmarkedWords a set of words that the user has bookmarked
     * @param messageData the message data to be scored
     **/
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        int score = 0;
        for (String w : messageData.getWords()) {
            if (bookmarkedWords.contains(w)) {
                score++;
            }
        }
        return score;
    }
    /**
     * Compare two messages based on their score.
     *
     * @param leftData the left message Data
     * @param leftMessage the left message
     * @param rightData the right message data
     * @param rightMessage the right message
     * */
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
