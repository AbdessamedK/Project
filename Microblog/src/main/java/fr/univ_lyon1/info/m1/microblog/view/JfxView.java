package fr.univ_lyon1.info.m1.microblog.view;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.LinkedHashMap;
import java.util.Map.Entry;

import fr.univ_lyon1.info.m1.microblog.controller.Controller;
import fr.univ_lyon1.info.m1.microblog.model.Message;
import fr.univ_lyon1.info.m1.microblog.model.MessageData;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.User;
import fr.univ_lyon1.info.m1.microblog.model.Y;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextArea;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Priority;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

/**
 * Main class of the View (GUI) of the application.
 */
public class JfxView implements PropertyChangeListener {
    private final Y model;
    private final HBox users;
    private final Controller controller;
    private boolean sombre = true;
    /**
     * Main view of the application.
     */
    public JfxView(final Y y, final Stage stage, final int width, final int height) {
        stage.setTitle("Y Microblogging");

        this.model = y;

        this.controller = new Controller(y);
        
        final VBox root = new VBox(10);


        // final Pane search = createSearchWidget();
        // root.getChildren().add(search);

        users = new HBox(10);
        users.getStyleClass().add("userBox");

        root.getChildren().add(users);

        createUsersPanes();
        this.model.addListener(this);
        // Everything's ready: add it to the scene and display it
        final Scene scene = new Scene(root, width, height);
        scene.getStylesheets().add(getClass().getResource("/viewDark.css").toExternalForm());
        stage.setScene(scene);
        stage.show();
    }

    /**
     * Create the pane containing all user's, from the users' registry contained in the model.
     */
    private void createUsersPanes() {
        users.getChildren().clear();

        for (User u : model.getUsers()) {
            ScrollPane p = new ScrollPane();
            //p.getStyleClass().add("userBox");


            VBox userBox = new VBox(1);
            userBox.setAlignment(Pos.CENTER);

            userBox.getStyleClass().add("userBox");
            p.setMinWidth(300);
            p.setContent(userBox);
            users.getChildren().add(p);


            VBox userMsgBox = new VBox();
            
            ComboBox<ScoreStrategy> strategyBox = new ComboBox<>();
            strategyBox.getItems().addAll(model.getStrategys());
            
            strategyBox.setOnAction(e -> {
                controller.setStrategySelected(strategyBox.getValue(), u);
                update();
            });


            Label userID = new Label(u.getId());
            Label stratLabel = new Label("Strat au choix");
            stratLabel.setAlignment(Pos.TOP_RIGHT);
            Label modeLabel = new Label("Theme aux choix");
            modeLabel.setAlignment(Pos.TOP_RIGHT);

            ComboBox<ThemeStrategy> themeBox = new ComboBox<>();
            themeBox.getStyleClass().add("themeBox");
            themeBox.getItems().addAll(NormalTheme.getInstance(), 
                                        DarkTheme.getInstance()); 

            ThemeStrategy s = new ThemeFromString().createStrategy(u.getThemeStrategy());
            themeBox.setValue(s);
            themeBox.getValue().applyTheme(userBox);
            themeBox.setOnAction(e -> {
                controller.setThemeSelected(themeBox.getValue(), u);
                themeBox.getValue().applyTheme(userBox);
            });


            //pour tester
            //sombre = !sombre;
            //userBox.getStyleClass().add("label");
            // if (sombre) {
            //     userBox.getStyleClass().add("dark-mode");
            //     userBox.getStyleClass().remove("light-mode");
            // } else {
            //     userBox.getStyleClass().remove("dark-mode");
            //     userBox.getStyleClass().add("light-mode");
            // }
            

            //themeBox.setOnAction(e -> {
            //     ThemeStrategy selectedTheme = themeBox.getValue();
            //     if (selectedTheme != null) {
            //         selectedTheme.applyTheme(userBox.getScene()); // Applique le thème à la scène
            //     }
            // });

            Pane textBox = createInputWidget(u);
            //textBox.getStyleClass().add("userBox");

            userBox.getChildren().addAll(userID, stratLabel, strategyBox, 
                                        modeLabel, themeBox, userMsgBox, textBox);
        }
    }

    @Override
    public void propertyChange(final PropertyChangeEvent e) {
       update();
    }

    /** Updates the message fields for each user. */
    public void update() {
        for (User u : model.getUsers()) {
            setMessageList(model.getMessages(u), u);
        }
    }

    private void setMessageList(final LinkedHashMap<Message, MessageData> messagesData, 
                                final User user) {
        for (Node u : users.getChildren()) {
            ScrollPane scroll = (ScrollPane) u;
            VBox userBox = (VBox) scroll.getContent();
            Label userID = (Label) userBox.getChildren().get(0);
            if (userID.getText().equals(user.getId())) {
                VBox userMsg = (VBox) userBox.getChildren().get(5);
                userMsg.getChildren().clear();
                for (Entry<Message, MessageData> e : messagesData.entrySet()) {
                        Message m = e.getKey();
                        MessageData d = e.getValue();
                        userMsg.getChildren().add(createMessageWidget(m, d, user));
                }
                break;
            }
        }
    }

    static final String MSG_STYLE = 
             "-fx-border-color: grey; -fx-border-width: 1;"
            + "-fx-border-radius: 10px;"
            + "-fx-background-radius: 10px;"
            + "-fx-padding: 8px; "
            + "-fx-margin: 5px; ";


    private VBox createMessageWidget(final Message m, final MessageData d, final User u) {
        VBox msgBox = new VBox();
        msgBox.getStyleClass().add("userMsgBox");
        //msgBox.getStyleClass().add("dark-mode");
        HBox buttonBox = new HBox();
        String bookmarkText;
        boolean bookmarked = d.isBookmarked();
        if (bookmarked) {
            bookmarkText = "⭐";
        } else {
            bookmarkText = "Click to bookmark";
        }
        Button bookButton = new Button(bookmarkText);
        bookButton.getStyleClass().add("bookButton");

        bookButton.setOnAction(e -> {
            if (bookButton.getText().equals("Click to bookmark")) {
                bookButton.setText("⭐");
                this.controller.setBookmarked(m, u, true);
            } else {
                bookButton.setText("Click to bookmark");
                this.controller.setBookmarked(m, u, false);
            }
        });
        buttonBox.getChildren().add(bookButton);
        

            
        Label nameUser = new Label("@" + m.getOwner().getId());
        Region spacer = new Region();
        HBox.setHgrow(spacer, Priority.ALWAYS);

        if (model.canDelete(u, m)) {
            String deleteButtontext = "x";
            Button deleteButton = new Button(deleteButtontext);
            deleteButton.getStyleClass().add("deleteButton");

            deleteButton.setOnAction(e -> {
                this.controller.deleteMessage(m);
            });
            buttonBox.getChildren().add(deleteButton);
        }
            
            
        HBox boxIdUser = new HBox(nameUser, spacer, buttonBox);

        msgBox.getChildren().addAll(boxIdUser);
        


        //ICI ON AJOUTE POUR QUE CA S'AFFICHE
        final Label label = new Label(m.getContent());
        msgBox.getChildren().add(label);

        final Label date = new Label(m.getDatePublish());
        date.getStyleClass().add("labelOther");
        msgBox.getChildren().add(date);

        final Label score = new Label("Score: " + d.getScore());
        score.getStyleClass().add("labelOther");
        msgBox.getChildren().add(score);

        msgBox.setStyle(MSG_STYLE);
        return msgBox;
    }

    private Pane createInputWidget(final User u) {
        final Pane input = new HBox();  
        TextArea t = new TextArea();
        t.setMaxSize(200, 150);
        t.setOnKeyPressed(e -> {
            if (e.getCode() == KeyCode.ENTER && e.isControlDown() 
                && !(t.getText().trim().isEmpty())) {
                publish(t, u);
                t.clear();
            }
        });
        Button s = new Button("Publish");
        s.getStyleClass().add("publishButton");

        s.setDisable(true);
        t.textProperty().addListener((observable, oldValue, newValue) -> {
            s.setDisable(newValue.trim().isEmpty());
        });

        s.setOnAction(e -> {
            publish(t, u);
            t.clear();
        });
        input.getChildren().addAll(t, s);
        return input;
    }

    private void publish(final TextArea t, final User u) {
        this.controller.addMessage(new Message(t.getText(), u));
    }


}
