package fr.univ_lyon1.info.m1.microblog.model.strategy;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.util.Set;

/**
 * Abstract Scoring of messages .
 */
public interface ScoreStrategy {
    /**
     * Calculate the score for a message.
     *
     * @param m the message
     * @return the score
     * */
    int calculateScore(Message m, Set<String> bookmarkedWords, MessageData messageData);

    /**
     * Compare two messages based on their score.
     *
     * @param leftData the left message Data
     * @param leftMessage the left message
     * @param rightData the right message data
     * @param rightMessage the right message
     * */
    int compare(Message leftMessage,
                MessageData leftData,
                Message rightMessage,
                MessageData rightData);

}
