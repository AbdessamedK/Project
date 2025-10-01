package fr.univ_lyon1.info.m1.microblog.model;

import java.io.FileReader;
import java.io.IOException;
import java.util.Calendar;
import java.util.Date;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/**
 * A class who create an example set of users and messages, for testing.
 * Contained in the initConfig.json file in src/main/resources.
 */
public class ExampleMessages {
    private final Y model;

    /**
     * Constructor that generates the contents of the config file.
     * @param y the model where the tests should be generated
     */
    public ExampleMessages(final Y y) {
        this.model = y;
        createExampleMessages();
    }

    private void createExampleMessages() {
        JSONParser parser = new JSONParser();
        try {
            JSONObject data = (JSONObject) parser.parse(
                new FileReader("src/main/resources/initConfig.json")
            );
            JSONArray users = (JSONArray) data.get("users");
            JSONArray messages = (JSONArray) data.get("messages");

            for (Object obj : users) {
                JSONObject user = (JSONObject) obj;
                String id = (String) user.get("id");
                Role role = Role.valueOf((String) user.get("role"));
                model.createUser(id, role);
            }

            for (Object obj : messages) {
                JSONObject message = (JSONObject) obj;
                String content = (String) message.get("content");
                String date = (String) message.get("date");
                String owner = (String) message.get("owner");

                if (date.equals("now")) {
                    model.add(new Message(content, model.getUser(owner)));
                } else {
                    int minusDay = Integer.parseInt(date);
                    Calendar calendar = Calendar.getInstance();
                    calendar.add(Calendar.DAY_OF_MONTH, minusDay);

                    Date minusDate = calendar.getTime();
                    model.add(new Message(content, minusDate, model.getUser(owner)));
                }
            }

        } catch (IOException | ParseException e) {
            System.out.println("Erreur : " + e);
        }
    }
}
