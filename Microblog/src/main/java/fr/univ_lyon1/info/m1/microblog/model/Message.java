package fr.univ_lyon1.info.m1.microblog.model;

import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Message and its own data.
 */
public class Message {
    private final String content;
    private final String datePublish;
    private int id;
    private final User owner;


    public String getContent() {
        return content;
    }
    public String getDatePublish() {
        return datePublish;
    }

    public int getId() {
        return id;
    }

    public void setId(final int idMessage) {
        this.id = idMessage;
    }

    public User getOwner() {
        return this.owner;
    }
    
    /**
     * Build a Message object from it's (String) content
     * and the owner of the message.
     */
    public Message(final String content, final User u) {
        this.content = content;
        //ici on initialise le format de la date puis avec newDate() on recupére la date récente
        String pattern = "dd/MM/yyyy • HH:mm:ss:SSS";
        SimpleDateFormat sdf = new SimpleDateFormat(pattern);
        this.datePublish = sdf.format(new Date());
        this.owner = u;
    }
    
    /**
     * Build a Message object from it's (String) content with possibility to personalize the date.
     */
    public Message(final String content, final Date date, final User u) {
        this.content = content;
        //ici on initialise le format de la date puis avec newDate() on recupére la date récente
        String pattern = "dd/MM/yyyy • HH:mm:ss:SSS";
        SimpleDateFormat sdf = new SimpleDateFormat(pattern);
        this.datePublish = sdf.format(date);
        this.owner = u;
    }
}
