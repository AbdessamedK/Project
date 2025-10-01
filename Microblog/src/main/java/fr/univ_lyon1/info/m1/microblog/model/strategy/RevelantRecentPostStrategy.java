package fr.univ_lyon1.info.m1.microblog.model.strategy;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.Date;
import java.util.Set;


/**
 * Strategy to calculate the score for a message based on the presence of
 * bookmarked words in the message data and the date of the message.
 */
public final class RevelantRecentPostStrategy implements ScoreStrategy {
    private static RevelantRecentPostStrategy instance;

    private RevelantRecentPostStrategy() { }

    /** Function that returns only one instance of the class. */
    public static RevelantRecentPostStrategy getInstance() {
        if (instance == null) {
            instance = new RevelantRecentPostStrategy();
        }
        return instance;
    }

    /**
     * Calculates the score for a given message based on the presence of
     * bookmarked words in the message data.
     *
     * @param m the message to be scored
     * @return the calculated score, which is the count of bookmarked words found
     *         in the message data
     */
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        ScoreStrategy recent = new ScoreStrategyFromString().createStrategy(
                "RecentPostStrategy"
                        );
        ScoreStrategy bookmark = new ScoreStrategyFromString().createStrategy(
                "BookmarkedWordStrategy"
                        );
        int score = recent.calculateScore(m, bookmarkedWords, messageData)
                +   bookmark.calculateScore(m, bookmarkedWords, messageData);
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
            SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy • HH:mm:ss:SSS");
            try {
                Date leftMessageDate = sdf.parse(leftMessage.getDatePublish());

                Date rightMessageDate = sdf.parse(rightMessage.getDatePublish());

                LocalDateTime leftDateTime = leftMessageDate.toInstant()
                        .atZone(ZoneId.systemDefault())
                        .toLocalDateTime();

                LocalDateTime rightDateTime = rightMessageDate.toInstant()
                        .atZone(ZoneId.systemDefault())
                        .toLocalDateTime();

                if (leftDateTime.isAfter(rightDateTime)) {
                    return 1;
                } else if (rightDateTime.isAfter(leftDateTime)) {
                    return -1;
                } else {
                    return 0;
                }
            } catch (ParseException e) {
                System.out.println("Erreur de parsing : " + e.getMessage());
                return 0;
            }
        } else if (scoreLeft > scoreRight) {
            return 1;
        }
        throw new AssertionError("Case not covered in comparision");
    }
    @Override
    public String toString() {
        return "Messages pertinents récents";
    }
}
