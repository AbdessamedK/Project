package fr.univ_lyon1.info.m1.microblog.model.strategy;

import java.time.LocalDate;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.contains;
import static org.hamcrest.Matchers.is;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;
import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.User;
import org.junit.jupiter.api.Test;
/**
 *  Test message list manipulations with BookMarked Word Strategy.
 */
public class TestSortMessagesBookmarkedWordStrategy {
               @Test
        void test() {
                // Given
                Map<Message, MessageData> msgs = new HashMap<>();
                List<Message> sortedMessageList = new ArrayList<>();

                LocalDate today = LocalDate.now();
                LocalDate dateMinus2Days = today.minusDays(2);
                LocalDate dateMinus2Weeks = today.minusWeeks(2);
                LocalDate dateMinus3Days = today.minusDays(3);
                LocalDate dateMinus3Weeks = today.minusWeeks(3);
                Date dateMinus2DaysAsDate = Date.from(
                dateMinus2Days
                .atStartOfDay(ZoneId.systemDefault())
                .toInstant());
                Date dateMinus3DaysAsDate = Date.from(
                dateMinus3Days
                .atStartOfDay(ZoneId.systemDefault())
                .toInstant());
                Date dateMinus2WeeksAsDate = Date.from(
                dateMinus2Weeks
                .atStartOfDay(ZoneId.systemDefault())
                .toInstant());
                Date dateMinus3WeeksAsDate = Date.from(
                dateMinus3Weeks
                .atStartOfDay(ZoneId.systemDefault())
                .toInstant());
                
                User temp = new User("temp", Role.USER);

                Message m1 = new Message("spam, is fake !", temp);
                add(msgs, m1);
                Message m2 = new Message("spam, you!", dateMinus3WeeksAsDate, temp);
                add(msgs, m2);
                Message m3 = new Message("What is this message ?", dateMinus3DaysAsDate, temp);
                add(msgs, m3);
                Message m4 = new Message("This is message with score 1", 
                        dateMinus2DaysAsDate, 
                        temp
                );
                add(msgs, m4);
                Message m5 = new Message("This is message with score 0", 
                        dateMinus2WeeksAsDate, 
                        temp
                );
                add(msgs, m5); 


                msgs.get(m1).setBookmarked(true);
                msgs.get(m5).setBookmarked(true);

                ScoreStrategy s = new ScoreStrategyFromString().createStrategy(
                        "BookmarkedWordStrategy"
                                );
                // When
                new BookmarkScoring().computeScores(msgs, s);

                msgs.entrySet()
                .stream()
                .sorted(Collections.reverseOrder((Entry<Message, MessageData> left,
                        Entry<Message, MessageData> right) -> {
                        return s.compare(left.getKey(),
                                                left.getValue(),
                                                right.getKey(),
                                                right.getValue());
                }))
                .forEach((Entry<Message, MessageData> e) -> {
                        if (e.getValue().getScore() > 0) {
                        sortedMessageList.add(e.getKey());
                        }
                });

                // Then
                assertThat(msgs.get(m1).getScore(), is(3));
                assertThat(msgs.get(m2).getScore(), is(1));
                assertThat(msgs.get(m3).getScore(), is(3));
                assertThat(msgs.get(m4).getScore(), is(5));
                assertThat(msgs.get(m5).getScore(), is(5));
                
                assertThat(sortedMessageList, contains(m5, m1, m4, m3, m2));
        }

        private void add(final Map<Message, MessageData> msgs, final Message m) {
                msgs.put(m, new MessageData(null, m));
        }   
}
