package fr.univ_lyon1.info.m1.microblog.model.strategy;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Scoring of messages based on bookmarks.
 */
public class BookmarkScoring {


    /** Compute the score for all messages in messagesData. */
    public void computeScores(final Map<Message, MessageData> messagesData,
                              final ScoreStrategy s) {
        Set<Message> messages = messagesData.keySet();
        Set<String> bookmarkedWords = new HashSet<>();

        messages.forEach((Message m) -> {
            MessageData d = messagesData.get(m);
            String[] w = m.getContent().toLowerCase().split("[^\\p{Alpha}]+");
           // String[] x = m.getDatePublish().toLowerCase().split("[^\\p{Alpha}]+");
            Set<String> words = new HashSet<>();
            words.addAll(Arrays.asList(w)); // For some reason, Set.of refuses duplicates
           // words.addAll(Arrays.asList(x)); // For some reason, Set.of refuses duplicates
            d.setWords(words);
            if (d.isBookmarked()) {
                bookmarkedWords.addAll(words);
            }
        });

        messages.forEach((Message m) -> {
            MessageData d = messagesData.get(m);
            int score = 0;
            //System.out.println("Score: " + d.getScore());
            score += s.calculateScore(m, bookmarkedWords, d);
            d.setScore(score);
        });
    }


}
