package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Test the UserDao class with a test for each method.
 */
public final class TestUserDao {
    
    private UserDao userDao;

    @BeforeAll
    @SuppressWarnings("unused")
    static void setUpDB() {
        String sql = "CREATE TABLE IF NOT EXISTS Users (" 
            + "id TEXT PRIMARY KEY,"
            + "strategy TEXT,"
            + "theme TEXT,"
            + "role TEXT)";
        QuerySql.query(sql);
    }

    @BeforeEach
    @SuppressWarnings("unused")
    void cleanTable() {
        userDao = new UserDao();
        String sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);
    }

    @Test
    void testAdd() {
        User user = new User("Test% toto", Role.ADMIN);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        user.setThemeStrategy("NormalTheme");
        userDao.add(user);

        String sql = "SELECT * FROM Users WHERE id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, user.getId());
        ResultSet res = result.getKey();

        try {
            String id = res.getString("id");
            String strategy = res.getString("strategy");
            String theme = res.getString("theme");
            Role role = Role.valueOf(res.getString("role"));

            result.getValue().close();

            assertEquals(id, "Test% toto");
            assertEquals(strategy, "ChronologicalPostStrategy");
            assertEquals(theme, "NormalTheme");
            assertEquals(role, Role.ADMIN);


        } catch (SQLException e) {
            System.out.println("Erreur testAdd UserDao !");
            assertEquals(true, false);
        }

    }

    @Test 
    void testUpdate() {
        User temp = new User("Test% toto", Role.ADMIN);
        temp.setStrategy(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        temp.setThemeStrategy("white");
        userDao.add(temp);

        User user = new User("Test% totoUpdate", Role.USER);
        user.setStrategy(new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"));
        user.setThemeStrategy("dark");

        userDao.update("Test% toto", user);

        String sql = "SELECT * FROM Users WHERE id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, user.getId());
        ResultSet res = result.getKey();

        try {
            String id = res.getString("id");
            String strategy = res.getString("strategy");
            String theme = res.getString("theme");
            Role role = Role.valueOf(res.getString("role"));

            result.getValue().close();

            assertEquals(id, "Test% totoUpdate");
            assertEquals(strategy, "RevelantPostStrategy");
            assertEquals(theme, "dark");
            assertEquals(role, Role.USER);


        } catch (SQLException e) {
            System.out.println("Erreur testUpdate UserDao !");
            assertEquals(true, false);
        }
    }

    @Test 
    void testFindOne() {
        User temp = new User("Test% totoUpdate", Role.ADMIN);
        temp.setStrategy(new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"));
        temp.setThemeStrategy("dark");
        userDao.add(temp);

        User user = userDao.findOne("Test% totoUpdate");

        assertEquals("Test% totoUpdate", user.getId());
        assertEquals(
            new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"), 
            user.getStrategy()
        );
        assertEquals("dark", user.getThemeStrategy());
        assertEquals(Role.ADMIN, user.getRole());
    }

    @Test 
    void testDelete() {
        User temp = new User("Test% totoUpdate", Role.ADMIN);
        temp.setStrategy(new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"));
        temp.setThemeStrategy("dark");
        userDao.add(temp);

        User user = userDao.findOne("Test% totoUpdate");
        userDao.delete(user);

        String sql = "SELECT * FROM Users WHERE id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, user.getId());
        ResultSet res = result.getKey();

        try {
            assertFalse(res.next());
            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur testDelete UserDao !");
            assertEquals(true, false);
        }
    }

    @Test 
    void testFindAll() {
        List<User> users = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            User user = new User("Test% toto" + i, Role.USER);
            user.setStrategy(new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"));
            user.setThemeStrategy("dark");
            
            users.add(user);
            userDao.add(user);
        }

        List<User> usersFindAll = (List<User>) userDao.findAll()
            .stream()
            .filter(u -> u.getId().startsWith("Test%"))
            .collect(Collectors.toList());

        assertEquals(users.size(), usersFindAll.size());

        for (int i = 0; i < 4; i++) {
            assertEquals(users.get(i).getId(), usersFindAll.get(i).getId());
            assertEquals(users.get(i).getStrategy(), usersFindAll.get(i).getStrategy());
            assertEquals(users.get(i).getThemeStrategy(), usersFindAll.get(i).getThemeStrategy());
            assertEquals(users.get(i).getRole(), Role.USER);
        }
    }

    @AfterAll
    @SuppressWarnings("unused")
    static void clean() {
        String sql = "DELETE FROM Users WHERE id LIKE 'Test%'";
        QuerySql.query(sql);
    }
}
