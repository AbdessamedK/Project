package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import fr.univ_lyon1.info.m1.microblog.model.strategy.BookmarkScoring;
import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;
import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Test the MessageDataDao class with a test for each method
 * except delete because it is empty.
 */
public final class TestMessageDataDao {
    
    private MessageDataDao messageDataDao;

    private String getStringFromSet(final Set<String> words) {
        return String.join(";", words);
    }

    @BeforeAll
    @SuppressWarnings("unused")
    static void setUp() {
        String sql = "CREATE TABLE IF NOT EXISTS MessageData (" 
            + "user_id TEXT,"  
            + "message_id TEXT," 
            + "bookmarked TEXT," 
            + "score TEXT," 
            + "words TEXT," 
            + "PRIMARY KEY (user_id, message_id)," 
            + "FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE," 
            + "FOREIGN KEY (message_id) REFERENCES Message(id) ON DELETE CASCADE)";
        QuerySql.query(sql);
    }

    @BeforeEach
    @SuppressWarnings("unused")
    void cleanTable() {
        UserDao userDao = new UserDao();
        messageDataDao = new MessageDataDao(userDao, new MessageDao(userDao));

        String sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);

        sql = "DELETE FROM Message WHERE content LIKE 'Test%'";
        QuerySql.query(sql);
    }

    @Test 
    void testAdd() {
        User toto = new User("Test% toto", Role.USER);
        toto.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        toto.setThemeStrategy("NormalTheme");
        UserDao userDao = new UserDao();
        userDao.add(toto);

        Message message = new Message("Test% Hello World !", toto);
        new MessageDao(userDao).add(message);

        MessageData messageData = new MessageData(toto, message);
        messageDataDao.add(messageData);

        String sql = "SELECT * FROM MessageData WHERE user_id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, toto.getId());
        ResultSet res = result.getKey();

        try {
            String userId = res.getString("user_id");
            int messageId = Integer.parseInt(res.getString("message_id"));
            boolean bookmarked = Boolean.parseBoolean(res.getString("bookmarked"));
            int score = Integer.parseInt(res.getString("score"));
            String words = res.getString("words");

            result.getValue().close();

            Pair<User, Message> id = messageData.getId();

            assertEquals(userId, id.getKey().getId());
            assertEquals(messageId, id.getValue().getId());
            assertEquals(bookmarked, messageData.isBookmarked());
            assertEquals(score, messageData.getScore());
            assertEquals(words, getStringFromSet(messageData.getWords()));

        } catch (SQLException e) {
            System.out.println("Erreur testAdd MessageDataDao ! ");
            assertFalse(true);
        }
    }

    @Test 
    void testUpdate() {
        User toto = new User("Test% toto", Role.USER);
        toto.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        toto.setThemeStrategy("NormalTheme");
        UserDao userDao = new UserDao();
        userDao.add(toto);

        Message message = new Message("Test% Hello World !", toto);
        new MessageDao(userDao).add(message);

        MessageData messageData = new MessageData(toto, message);
        messageDataDao.add(messageData);
        
        messageData.setBookmarked(true);

        Map<Message, MessageData> msgs = new HashMap<>();
        msgs.put(message, messageData);
        
        new BookmarkScoring().computeScores(msgs, toto.getStrategy());

        messageDataDao.update(new Pair<>(toto, message), messageData);

        String sql = "SELECT * FROM MessageData WHERE user_id = ? AND message_id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(
            sql, toto.getId(), 
            String.valueOf(message.getId())
        );

        ResultSet res = result.getKey();
        try {
            String userId = res.getString("user_id");
            int messageId = Integer.parseInt(res.getString("message_id"));
            boolean bookmarked = Boolean.parseBoolean(res.getString("bookmarked"));
            int score = Integer.parseInt(res.getString("score"));
            String words = res.getString("words");

            result.getValue().close();

            Pair<User, Message> id = messageData.getId();

            assertEquals(userId, id.getKey().getId());
            assertEquals(messageId, id.getValue().getId());
            assertEquals(bookmarked, messageData.isBookmarked());
            assertEquals(score, messageData.getScore());
            assertEquals(words, getStringFromSet(messageData.getWords()));

        } catch (SQLException e) {
            System.out.println("Erreur testUpdate MessageDataDao !");
            assertFalse(true);
        }
    }

    @Test 
    void testFindAll() {
        User toto = new User("Test% toto", Role.USER);
        toto.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        toto.setThemeStrategy("NormalTheme");
        
        UserDao userDao = new UserDao();
        userDao.add(toto);

        MessageDao messageDao = new MessageDao(userDao);

        List<MessageData> messageDatas = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            Message m = new Message("Test% Hello World !", toto);
            MessageData messageData = new MessageData(toto, m);
            
            messageDao.add(m);
            messageDataDao.add(messageData);
            messageDatas.add(messageData);
        }

        List<MessageData> messageDatasFindAll = (List<MessageData>) messageDataDao.findAll()
            .stream()
            .filter(msgData -> msgData.getId().getKey().getId().startsWith("Test%"))
            .collect(Collectors.toList());

        assertEquals(messageDatas.size(), messageDatasFindAll.size());

        for (int i = 0; i < 4; i++) {
            Pair<User, Message> id = messageDatas.get(i).getId();
            Pair<User, Message> idFindAll = messageDatasFindAll.get(i).getId();

            assertEquals(id.getKey().getId(), idFindAll.getKey().getId());
            assertEquals(id.getValue().getId(), idFindAll.getValue().getId());
            assertEquals(messageDatas.get(i).getScore(), messageDatasFindAll.get(i).getScore());
            
            assertEquals(
                messageDatas.get(i).isBookmarked(), 
                messageDatasFindAll.get(i).isBookmarked()
            );
            
            assertEquals(
                getStringFromSet(messageDatas.get(i).getWords()), 
                getStringFromSet(messageDatasFindAll.get(i).getWords())
            );
        }
    }

    @Test 
    void testFindOne() {
        User toto = new User("Test% toto", Role.USER);
        toto.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        toto.setThemeStrategy("NormalTheme");
        UserDao userDao = new UserDao();
        userDao.add(toto);

        Message message = new Message("Test% Hello World !", toto);
        new MessageDao(userDao).add(message);

        MessageData messageData = new MessageData(toto, message);
        messageDataDao.add(messageData);
        
        Pair<User, Message> id = messageData.getId();
        MessageData messageDataFind = messageDataDao.findOne(id);

        Pair<User, Message> idFindOne = messageDataFind.getId();

        assertEquals(id.getKey().getId(), idFindOne.getKey().getId());
        assertEquals(id.getValue().getId(), idFindOne.getValue().getId());
        assertEquals(messageData.getScore(), messageDataFind.getScore());

        assertEquals(
            messageData.isBookmarked(),
            messageDataFind.isBookmarked()
        );

        assertEquals(
            getStringFromSet(messageData.getWords()),
            getStringFromSet(messageDataFind.getWords())
        );
    }

    @AfterAll
    @SuppressWarnings("unused")
    static void clean() {
        String sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);

        sql = "DELETE FROM Message WHERE content LIKE 'Test%'";
        QuerySql.query(sql);
    }
}
