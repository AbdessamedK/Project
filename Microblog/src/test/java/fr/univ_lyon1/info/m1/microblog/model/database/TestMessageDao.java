package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Test the MessageDao class with a test for each method.
 */
public final class TestMessageDao {
    
    private MessageDao messageDao;

    @BeforeAll
    @SuppressWarnings("unused")
    static void setUpDB() {
        String sql = "CREATE TABLE IF NOT EXISTS Message ("
            + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            + "owner_id TEXT,"
            + "content TEXT," 
            + "date TEXT,"
            + "FOREIGN KEY (owner_id) REFERENCES Users(id))";
        QuerySql.query(sql);
    }

    @BeforeEach
    @SuppressWarnings("unused")
    void cleanTable() {
        UserDao userDao = new UserDao();
        messageDao = new MessageDao(userDao);
        String sql = "DELETE FROM Message WHERE content LIKE 'Test%'";
        QuerySql.query(sql);
        sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);
    }

    @Test
    void testAdd() {
        User user = new User("Test% toto", Role.USER);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        new UserDao().add(user);

        Date now = new Date();
        Message message = new Message("Test% Hello World !", now, user);
        
        messageDao.add(message);

        assertNotNull(message.getId());

        String sql = "SELECT * FROM Message WHERE id = ?";
        String id = String.valueOf(message.getId());
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, id);
        ResultSet res = result.getKey();
        try {
            String idString = res.getString("id");
            String content = res.getString("content");
            String dateString = res.getString("date");
            String owner = res.getString("owner_id");

            result.getValue().close();

            assertEquals(id, idString);
            assertEquals("Test% Hello World !", content);

            String pattern = "dd/MM/yyyy • HH:mm:ss:SSS";
            SimpleDateFormat sdf = new SimpleDateFormat(pattern);
            String datePublish = sdf.format(now);

            assertEquals(datePublish, dateString);
            assertEquals(owner, user.getId());

        } catch (SQLException e) {
            System.out.println("Erreur testAdd MessageDao !");
            assertEquals(true, false);
        }
    }

    @Test
    void testUpdate() {
        User user = new User("Test% toto", Role.USER);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        new UserDao().add(user);

        Message message = new Message("Test% Hello World !", user);
        messageDao.add(message);

        assertNotNull(message.getId());
        
        Message newMessage = new Message("Test% Update Hello World !", user);
        newMessage.setId(message.getId());
        messageDao.update(message.getId(), newMessage);

        String sql = "SELECT * FROM Message WHERE id = ?";
        String id = String.valueOf(message.getId());

        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, id);
        ResultSet res = result.getKey();

        try {
            String content = res.getString("content");
            String owner = res.getString("owner_id");
            result.getValue().close();

            assertEquals(newMessage.getContent(), content);
            assertEquals(owner, user.getId());

        } catch (SQLException e) {
            System.out.println("Erreur testUpdate MessageDao !");
            assertEquals(true, false);
        }
    }

    @Test
    void testfindOne() {
        User user = new User("Test% toto", Role.ADMIN);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        new UserDao().add(user);

        Message message = new Message("Test% Hello World !", user);
        messageDao.add(message);
        Message messageFind = messageDao.findOne(message.getId());

        assertEquals(message.getContent(), messageFind.getContent());
        assertEquals(message.getId(), messageFind.getId());
        assertEquals(message.getOwner().getId(), user.getId());
    }

    @Test
    void testFindAll() {
        User user = new User("Test% toto", Role.ADMIN);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        new UserDao().add(user);

        List<Message> messages = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            messages.add(new Message("Test% Hello World !" + i, user));
            messageDao.add(messages.get(i));
        }
        
        List<Message> messagesFindAll = (List<Message>) messageDao.findAll()
            .stream()
            .filter(m -> m.getContent().startsWith("Test%"))
            .collect(Collectors.toList());
        
        assertEquals(messages.size(), messagesFindAll.size());

        for (int i = 0; i < 4; i++) {
            assertEquals(messages.get(i).getContent(), messagesFindAll.get(i).getContent());
            assertEquals(
                messages.get(i).getOwner().getId(), 
                messagesFindAll.get(i).getOwner().getId()
            );
        }

    }

    @Test 
    void testDelete() {
        User user = new User("Test% toto", Role.ADMIN);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        new UserDao().add(user);

        Message message = new Message("Test% Hello World !", user);
        
        messageDao.add(message);
        messageDao.delete(message);

        String sql = "SELECT * FROM Message WHERE id = ?";
        String id = String.valueOf(message.getId());
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, id);
        ResultSet res = result.getKey();

        try {
            assertFalse(res.next());
            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur testDelete MessageDao !");
            assertEquals(false, true);
        }
    }

    @AfterAll
    @SuppressWarnings("unused")
    static void clean() {
        String sql = "DELETE FROM Message WHERE content LIKE 'Test%'";
        QuerySql.query(sql);
        sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);
    }
}
