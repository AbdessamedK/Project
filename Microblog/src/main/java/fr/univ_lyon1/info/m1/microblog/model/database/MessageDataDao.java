package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Data Access Object (DAO) for the MessageData class.
 * from a SQLite database.
 */
public class MessageDataDao implements Dao<Pair<User, Message>, MessageData> {
    private final UserDao userDao;
    private final MessageDao messageDao;

    /** Constructor that instantiates other Dao for foreign key references. */
    public MessageDataDao(final UserDao usrDao, final MessageDao msgDao) {
        this.userDao = usrDao;
        this.messageDao = msgDao;
    }

    private Map<String, String> stringFromMessageData(final MessageData messageData) {
        Map<String, String> map = new HashMap<>();
        
        map.put("userId", messageData.getId().getKey().getId());
        map.put("msgId", String.valueOf(messageData.getId().getValue().getId()));
        map.put("book", String.valueOf(messageData.isBookmarked()));
        map.put("score", String.valueOf(messageData.getScore()));
        map.put("words", String.join(";", messageData.getWords()));
        
        return map;
    }

    private MessageData createMessageData(final User user, final Message message,
        final String bookmarked, final String score, final String words) {
        
        MessageData messageData = new MessageData(user, message);
        messageData.setBookmarked(Boolean.parseBoolean(bookmarked));
        messageData.setScore(Integer.parseInt(score));
        Set<String> setWords = new HashSet<>(Arrays.asList(words.split(";")));
        messageData.setWords(setWords);

        return messageData;
    }

    @Override
    public void add(final MessageData messageData) {
        Map<String, String> params = stringFromMessageData(messageData);

        String sql = "INSERT OR IGNORE INTO MessageData (user_id, message_id,"
            + " bookmarked, score, words) VALUES (?, ?, ?, ?, ?)";
        
        QuerySql.query(
            sql, params.get("userId"),
            params.get("msgId"), params.get("book"), 
            params.get("score"), params.get("words")
        );
    }

    @Override
    public void update(final Pair<User, Message> id, final MessageData messageData) {
        Map<String, String> params = stringFromMessageData(messageData);

        String sql = "UPDATE MessageData SET user_id = ?, " 
            + "message_id = ?, bookmarked = ?, score = ?, "
            + "words = ? WHERE user_id = ? AND message_id = ?";

        QuerySql.query(
            sql, params.get("userId"), 
            params.get("msgId"), params.get("book"), 
            params.get("score"), params.get("words"),
            id.getKey().getId(), String.valueOf(id.getValue().getId())
        );
    }

    @Override
    public Collection<MessageData> findAll() {
        Collection<MessageData> messageDatas = new ArrayList<>();
        String sql = "SELECT * FROM MessageData";

        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql);
        ResultSet res = result.getKey();

        try {
            while (res.next()) { 
                User user = userDao.findOne(res.getString("user_id"));
                Message message = messageDao.findOne(Integer.valueOf(res.getString("message_id")));

                String bookmarked = res.getString("bookmarked");
                String score = res.getString("score");
                String words = res.getString("words");

                messageDatas.add(createMessageData(user, message, bookmarked, score, words));
            }
            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur recuperation MessageData : " + e);
        }

        return messageDatas;
    }

    @Override
    public MessageData findOne(final Pair<User, Message> id) {
        String sql = "SELECT * FROM MessageData WHERE user_id = ? AND message_id = ?";
        
        Pair<ResultSet, Connection> result = QuerySql.querySelect(
            sql, id.getKey().getId(), String.valueOf(id.getValue().getId())
        );
        ResultSet res = result.getKey();

        try {
            String bookmarked = res.getString("bookmarked");
            String score = res.getString("score");
            String words = res.getString("words");

            result.getValue().close();
            return createMessageData(id.getKey(), id.getValue(), bookmarked, score, words);

        } catch (SQLException e) {
            System.out.println("Erreur lors de la recuperation du MessageData : "
                + id.getKey().getId() + " " + String.valueOf(id.getValue().getId()));
        }
        return null;
    }

    /**
     * No need to do anything because it is automatically deleted 
     * when you delete the associated message or user.
     */
    @Override
    public void delete(final MessageData messageData) { }
}
