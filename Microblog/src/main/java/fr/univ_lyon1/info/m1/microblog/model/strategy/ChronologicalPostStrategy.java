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
 * Class representing a scoring strategy based on the age of messages.
 * This strategy calculates the score of a message based on its publication date,
 * favoring more recent messages.
 */
public final class ChronologicalPostStrategy implements ScoreStrategy {
    private static ChronologicalPostStrategy instance;
    
    private ChronologicalPostStrategy() { }

    /** Function that returns only one instance of the class. */
    public static ChronologicalPostStrategy getInstance() {
        if (instance == null) {
            instance = new ChronologicalPostStrategy();
        }
        return instance;
    }

    /**
     * Calculates the score of a message based on its publication date.
     *
     * @param m               The message for which the score is to be calculated.
     * @param bookmarkedWords A set of bookmarked words, not used in this strategy.
     * @param messageData     The data associated with the message, not used in this strategy.
     * @return An integer score representing the age of the message,
     * where a higher score indicates a more recent message.
     * If a parsing error occurs, returns Integer.MIN_VALUE.
     */
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        /*
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("dd MMMMM yyyy HH:mm:ss.SSSZ");
        try {
            Date messageDate = simpleDateFormat.parse(m.getDatePublish());
            long currentTime = System.currentTimeMillis();

            long ageInMs = currentTime - messageDate.getTime();

            long oneYearInMs = 365 * 24 * 60 * 60 * 1000;

            return (int) (oneYearInMs - ageInMs);
        } catch (Exception e) {
            System.out.println("Erreur de parsing : " + e.getMessage());
        }
        */

        return 1; // En cas d'erreur de parsing, renvoie une très faible valeur
    }

    /**
     * Returns a string representation of the strategy.
     *
     * @return A string describing the strategy ("Message par ordre chronologique").
     */
    @Override
    public String toString() {
        return "Message par ordre chronologique";
    }

    @Override
    public int compare(final Message leftMessage,
                       final MessageData leftData,
                       final Message rightMessage,
                       final MessageData rightData) {
        int scoreLeft = leftData.getScore();
        int scoreRight = rightData.getScore();


        if (scoreLeft < scoreRight) {
            return -1;
        } else if (scoreLeft == scoreRight) {
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat("dd/MM/yyyy • HH:mm:ss:SSS");
            try {
                Date leftMessageDate = simpleDateFormat.parse(leftMessage.getDatePublish());

                Date rightMessageDate = simpleDateFormat.parse(rightMessage.getDatePublish());

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
