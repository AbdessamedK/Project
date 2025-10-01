package fr.univ_lyon1.info.m1.microblog.model.strategy;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Set;

/**
 * Implements a scoring strategy for messages based on their recency.
 * This strategy calculates a score for a given message based on how recently
 * it was published, with higher scores for more recent posts.
 * 2+ for 24h and before
 * 1+ for 1 week and before
 **/
public final class RecentPostStrategy implements ScoreStrategy {
    private static RecentPostStrategy instance;

    private RecentPostStrategy() { }

    /** Function that returns only one instance of the class. */
    public static RecentPostStrategy getInstance() {
        if (instance == null) {
            instance = new RecentPostStrategy();
        }
        return instance;
    }
    /**
     * Calculates the score for a given message based on the recency of the message.
     *
     * @param m the message to be scored
     * @return the calculated score,
     * which is 2 if the message was published within the last 24 hours,
     * 1 if the message was published within the last 7 days.
     **/
    @Override
    public int calculateScore(final Message m,
                              final Set<String> bookmarkedWords,
                              final MessageData messageData) {
        Calendar calendar = Calendar.getInstance();
        calendar.add(Calendar.HOUR, -24);
        Date dateMinus24Hours = calendar.getTime();

        Calendar calendare = Calendar.getInstance();
        calendare.add(Calendar.DAY_OF_MONTH, -7);
        Date dateMinus7days = calendare.getTime();

        SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy • HH:mm:ss:SSS");
        try {
            Date messageDate = sdf.parse(m.getDatePublish());
            if (messageDate.after(dateMinus24Hours)) {
                return 2;
            } else if (messageDate.after(dateMinus7days)) {
                return 1;
            } else {
                return 0;
            }

        } catch (ParseException e) {
            System.out.println("Erreur de parsing : " + e.getMessage());
        }
        return 0;
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

    @Override
    public String toString() {
        return "Messages récents";
    }
}
