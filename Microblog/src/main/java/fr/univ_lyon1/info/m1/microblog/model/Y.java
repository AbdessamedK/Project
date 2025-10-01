package fr.univ_lyon1.info.m1.microblog.model;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import fr.univ_lyon1.info.m1.microblog.model.database.MessageDao;
import fr.univ_lyon1.info.m1.microblog.model.database.MessageDataDao;
import fr.univ_lyon1.info.m1.microblog.model.database.UserDao;
import fr.univ_lyon1.info.m1.microblog.model.strategy.BookmarkScoring;
import fr.univ_lyon1.info.m1.microblog.model.strategy.RevelantPostStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import javafx.util.Pair;

/**
 * Toplevel class for the Y microbloging application's model.
 */
public class Y {
    //private final Map<Message, Map<User, MessageData>> messages = new HashMap<>();

    private final UserDao users = new UserDao();
    private final MessageDao messages = new MessageDao(users);
    private final MessageDataDao messageDatas = new MessageDataDao(users, messages);
    
    private final PropertyChangeSupport support = new PropertyChangeSupport(this);

    /**
     * Adds a property change listener that will be notified of changes to the object's properties.
     *
     * @param listener the PropertyChangeListener to be added.
     *                 This listener will receive notifications when properties change.
     */
    public void addListener(final PropertyChangeListener listener) {
        this.support.addPropertyChangeListener(listener);
        this.support.firePropertyChange("update", null, null);
    }

    /** Set scoring method for specified user. */
    public void setScoringStrategy(final ScoreStrategy strategy, final User user) {
        user.setStrategy(strategy);
        users.update(user.getId(), user);

        Map<Message, MessageData> msgData = new HashMap<>();
        messages.findAll().forEach(msg -> {
            MessageData messageData = messageDatas.findOne(new Pair<>(user, msg));
            msgData.put(msg, messageData);
        });

        new BookmarkScoring().computeScores(msgData, strategy);

        for (MessageData data : msgData.values()) {
            messageDatas.update(data.getId(), data);
        }
        
        this.support.firePropertyChange("update", null, null);
    }

    /** Set theme method for specified user. */
    public void setThemeStrategy(final String strategy, final User user) {
        user.setThemeStrategy(strategy);
        users.update(user.getId(), user);

        this.support.firePropertyChange("update", null, null);
    }

    /** Get all existing strategies. */
    public List<ScoreStrategy> getStrategys() {
        List<ScoreStrategy> strategys = new ArrayList<>();
        strategys.add(new ScoreStrategyFromString().createStrategy("ChronologicalPostStrategy"));
        strategys.add(new ScoreStrategyFromString().createStrategy("RevelantPostStrategy"));
        strategys.add(new ScoreStrategyFromString().createStrategy("RevelantRecentPostStrategy"));
        return strategys;
    }

    /**
     * Create a user and add it to the user's registry.
     * @param id the id of the user
     */
    public void createUser(final String id, final Role role) {
        User user = new User(id, role);
        user.setStrategy(RevelantPostStrategy.getInstance());
        user.setThemeStrategy("NormalTheme");
        users.add(user);

        messages.findAll().forEach(msg -> {
            messageDatas.add(new MessageData(user, msg));
        });

        this.support.firePropertyChange("update", null, null);
    }

    /** Get all the users in the database. */
    public Collection<User> getUsers() {
        return users.findAll();
    }

    /**
     * Sets the bookmarked status of a specific message for a given user.
     * @param m the message to be bookmarked
     * @param u the user who wants to bookmark the message
     * @param bookmarked the desired bookmarked status (true to bookmark, false to unbookmark)
     */
    public void setBookmarked(final Message m, final User u, final boolean bookmarked) {
        Pair<User, Message> id = new Pair<>(u, m);
        MessageData messageData = messageDatas.findOne(id);
        messageData.setBookmarked(bookmarked);
        
        messageDatas.update(id, messageData);

        Map<Message, MessageData> msgData = new HashMap<>();
        messages.findAll().forEach(msg -> {
            msgData.put(msg, messageDatas.findOne(new Pair<>(u, msg)));
        });

        new BookmarkScoring().computeScores(msgData, u.getStrategy());

        for (MessageData data : msgData.values()) {
            messageDatas.update(data.getId(), data);
        }

        this.support.firePropertyChange("update", null, null);
    }

    /** Post a message. */
    public void add(final Message message) {
        messages.add(message);

        getUsers().forEach(user -> {
            MessageData messageData = new MessageData(user, message);            
            messageDatas.add(messageData);

            Map<Message, MessageData> msgData = new HashMap<>();
            msgData.put(message, messageData);

            new BookmarkScoring().computeScores(msgData, user.getStrategy());

            messageDatas.update(messageData.getId(), messageData);
            
        });

        this.support.firePropertyChange("update", null, null);
    }

    /**
     * Delete a message.
     * @param message the message to be deleted
     */
    public void delete(final Message message) {
        messages.delete(message);
        this.support.firePropertyChange("update", null, null);
    }

    /** Returns the User with the id as parameter. */
    public User getUser(final String id) {
        return users.findOne(id);
    }

    /**
     * Checks whether a user can delete a message or not.
     * @param user the user who wants to delete the message
     * @param message the message to delete
     * @return a boolean that allows or disallows the deletion of the message
     */
    public boolean canDelete(final User user, final Message message) {
        return (user.getRole().equals(Role.ADMIN)) 
            || (user.getId().equals(message.getOwner().getId()));
    }

    /**
     * Gets a sorted list of messages and associated data for a given user.
     *
     * @param user the user for whom messages are requested.
     * @return a LinkedHashMap containing the sorted messages
     *         and their associated data for the user.
     */
    public LinkedHashMap<Message, MessageData> getMessages(final User user) {
        Map<Message, MessageData> messageData = new HashMap<>();

        messages.findAll().forEach(msg -> {
            messageData.put(msg, messageDatas.findOne(new Pair<>(user, msg)));
        });

        LinkedHashMap<Message, MessageData> sortedMessageList = new LinkedHashMap<>();
        messageData.entrySet()
            .stream()
            .sorted(Collections.reverseOrder((Entry<Message, MessageData> left,
                    Entry<Message, MessageData> right) -> {
                return user.getStrategy().compare(left.getKey(),
                                             left.getValue(),
                                             right.getKey(),
                                             right.getValue());
            }))
            .forEach((Entry<Message, MessageData> e) -> {
                if (e.getValue().getScore() > 0) {
                    sortedMessageList.put(e.getKey(), e.getValue());
                }
            });

        return sortedMessageList;
    }
}
