package fr.univ_lyon1.info.m1.microblog.controller;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.Test;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;
import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import fr.univ_lyon1.info.m1.microblog.model.User;
import fr.univ_lyon1.info.m1.microblog.model.Y;
import fr.univ_lyon1.info.m1.microblog.model.database.QuerySql;
import fr.univ_lyon1.info.m1.microblog.view.ThemeFromString;

/** Test class for the Controller, verifying its interaction with the model. */
public class TestController {
    @Test 
    void test() {
        // Given
        Y model = new Y();
        Controller controller = new Controller(model);

        model.createUser("Test% toto", Role.USER);
        model.createUser("Test% admin", Role.ADMIN);

        User toto = model.getUser("Test% toto");
        User admin = model.getUser("Test% admin");

        Message m1 = new Message("Test% Hello World", toto);
        Message m2 = new Message("Test% Hello from admin", admin);


        // When
        controller.addMessage(m1);
        controller.addMessage(m2);
        controller.setStrategySelected(
            new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"),
            toto
        );
        controller.setBookmarked(m1, toto, true);
        controller.setThemeSelected(
            new ThemeFromString().createStrategy("DarkTheme"), 
            admin
        );
        controller.deleteMessage(m2);


        // Then
        Map<Message, MessageData> dataToto = model.getMessages(toto)
            .entrySet().stream()
            .filter(entry -> entry.getKey().getContent().startsWith("Test%"))
            .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));

        List<Message> messages = dataToto.keySet().stream()
            .collect(Collectors.toList());
        
        assertEquals(1, dataToto.size());
        
        assertEquals(
            new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"),
            model.getUser("Test% toto").getStrategy()
        );

        assertTrue(dataToto.get(messages.get(0)).isBookmarked());

        assertEquals(admin.getThemeStrategy(), model.getUser(admin.getId()).getThemeStrategy());

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
