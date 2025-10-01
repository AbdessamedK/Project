package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;

import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Data Access Object (DAO) for the Message class.
 * from a SQLite database.
 */
public class MessageDao implements Dao<Integer, Message> {
    private final UserDao userDao;

    /** Constructor that instantiates other Dao for foreign key references. */
    public MessageDao(final UserDao usrDao) {
        this.userDao = usrDao;
    }

    @Override
    public void add(final Message message) {
        String sql = "INSERT OR IGNORE INTO Message (owner_id, content, date) VALUES (?, ?, ?)";
        QuerySql.query(sql, 
            message.getOwner().getId(), 
            message.getContent(), 
            message.getDatePublish()
        );

        sql = "SELECT MAX(id) AS max_id FROM Message";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql);
        ResultSet res = result.getKey();
        try {
            String maxId = res.getString("max_id");
            message.setId(Integer.parseInt(maxId));
            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur lors de l'ajout du message : " + message.getContent());
        }
    }

    @Override
    public void update(final Integer id, final Message message) {
        String sql = "UPDATE Message SET id = ?, content = ?, date = ? WHERE id = ?";
        QuerySql.query(sql, String.valueOf(
                                message.getId()), 
                                message.getContent(), 
                                message.getDatePublish(), 
                                String.valueOf(id)
                            );
    }

    @Override
    public Collection<Message> findAll() {
        Collection<Message> messages = new ArrayList<>();
        String sql = "SELECT * FROM Message";

        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql);
        ResultSet res = result.getKey();
        try {
            while (res.next()) {
                String idString = res.getString("id");
                String content = res.getString("content");
                String dateString = res.getString("date");
                String ownerId = res.getString("owner_id");

                messages.add(createMessage(idString, content, dateString, ownerId));
            }

            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur lors de la recuperation des messages : " + e);
        }

        return messages;
    }

    @Override
    public Message findOne(final Integer id) {
        String sql = "SELECT * FROM Message WHERE id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, String.valueOf(id));
        ResultSet res = result.getKey();

        try {
            String idString = res.getString("id");
            String content = res.getString("content");
            String dateString = res.getString("date");
            String ownerId = res.getString("owner_id");

            result.getValue().close();
            return createMessage(idString, content, dateString, ownerId);

        } catch (SQLException e) {
            System.out.println("Erreur lors de la recuperation du messages : " 
                + String.valueOf(id));
        }
        return null;
    }

    private Message createMessage(final String idString, 
                                  final String content, 
                                  final String dateString,
                                  final String ownerId) {
        try {
            String pattern = "dd/MM/yyyy • HH:mm:ss:SSS";
            SimpleDateFormat sdf = new SimpleDateFormat(pattern);
            Date date = sdf.parse(dateString);

            int id = Integer.parseInt(idString);
            User owner = userDao.findOne(ownerId);

            Message message = new Message(content, date, owner);
            message.setId(id);

            return message;

        } catch (ParseException e) {
            System.out.println("Erreur de parsing : " + e);
        }
        return null;
    }

    @Override
    public void delete(final Message message) {
        String sql = "DELETE FROM Message WHERE id = ?";
        QuerySql.query(sql, String.valueOf(message.getId()));
    }
}
