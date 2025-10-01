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
 * Implements a scoring strategy for messages based on their length.
 */
public final class LengthPostStategy implements ScoreStrategy {
    private static LengthPostStategy instance;

    private LengthPostStategy() { }

    /** Function that returns only one instance of the class. */
    public static LengthPostStategy getInstance() {
        if (instance == null) {
            instance = new LengthPostStategy();
        }
        return instance;
    }
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        return m.getContent().length();
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

}
