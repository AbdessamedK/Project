package fr.univ_lyon1.info.m1.microblog.model;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import org.junit.jupiter.api.AfterAll;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import org.junit.jupiter.api.Test;

import fr.univ_lyon1.info.m1.microblog.model.database.QuerySql;

/** Tests the model using all interactions and different possible scenarios. */
public final class TestY {
    @Test 
    void test() {
        // Given
        Y model = new Y();
        
        model.createUser("Test% toto", Role.USER);
        model.createUser("Test% admin", Role.ADMIN);

        User toto = model.getUser("Test% toto");
        User admin = model.getUser("Test% admin");
        List<User> usersGiven = new ArrayList<>();
        usersGiven.add(toto);
        usersGiven.add(admin);

        assertEquals("Test% toto", toto.getId());
        assertEquals("Test% admin", admin.getId());

        Message m1 = new Message("Test% Hello World !", toto);
        Message m2 = new Message("Test% Hello from admin !", admin);

        List<ScoreStrategy> strategys = model.getStrategys();


        // When
        model.add(m1);
        model.add(m2);
        model.setScoringStrategy(
            new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"),
            toto
        );
        model.setThemeStrategy("DarkTheme", toto);
        model.setBookmarked(m2, toto, true);

        Map<Message, MessageData> dataToto = model.getMessages(toto)
            .entrySet().stream()
            .filter(entry -> entry.getKey().getContent().startsWith("Test%"))
            .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));

        List<Message> messages = dataToto.keySet().stream()
            .collect(Collectors.toList()); 


        // Then
        List<User> users = model.getUsers()
            .stream()
            .filter(user -> user.getId().startsWith("Test%"))
            .collect(Collectors.toList());

        assertEquals(2, users.size());
        for (int i = 0; i < users.size(); i++) {
            assertEquals(usersGiven.get(i).getId(), users.get(i).getId());
            assertEquals(usersGiven.get(i).getStrategy(), users.get(i).getStrategy());
            assertEquals(usersGiven.get(i).getThemeStrategy(), users.get(i).getThemeStrategy());
            assertEquals(usersGiven.get(i).getRole(), users.get(i).getRole());
        }

        assertEquals(strategys.size(), 3);
        assertFalse(model.canDelete(toto, m2));
        assertTrue(model.canDelete(admin, m1));
        assertTrue(model.canDelete(admin, m2));
        assertEquals(
            toto.getStrategy(), 
            new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy")
        );

        assertEquals(toto.getThemeStrategy(), "DarkTheme");
        assertNotEquals(
            dataToto.get(messages.get(0)).isBookmarked(),
            dataToto.get(messages.get(1)).isBookmarked()
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
