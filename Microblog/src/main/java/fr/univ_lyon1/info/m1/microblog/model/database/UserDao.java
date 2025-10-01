package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;

import fr.univ_lyon1.info.m1.microblog.model.Role;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import fr.univ_lyon1.info.m1.microblog.model.User;
import javafx.util.Pair;

/**
 * Data Access Object (DAO) for the User class.
 * from a SQLite database.
 */
public class UserDao implements Dao<String, User> {

    @Override
    public void update(final String id, final User user) {
        String sql = "UPDATE Users SET id = ?, strategy = ?, theme = ?, role = ? WHERE id = ?";
        String strategy = user.getStrategy().getClass().getSimpleName();
        QuerySql.query(sql, 
            user.getId(), 
            strategy, 
            user.getThemeStrategy(), 
            user.getRole().name(), 
            id
        );
    }

    /**
     * Create a user and add it to the database.
     * @param user the user to add
     */
    @Override
    public void add(final User user) {
        String sql = "INSERT OR IGNORE INTO Users (id, strategy, theme, role) VALUES (?, ?, ?, ?)";
        String strategy = user.getStrategy().getClass().getSimpleName();
        
        QuerySql.query(sql, user.getId(), strategy, user.getThemeStrategy(), user.getRole().name());
    }

    /** Get all the users in the database. */
    @Override
    public Collection<User> findAll() {
        Collection<User> users = new ArrayList<>();
        String sql = "SELECT * FROM Users";

        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql);
        ResultSet res = result.getKey();
        
        try {
            while (res.next()) {
                String id = res.getString("id");
                String strategyName = res.getString("strategy");
                String theme = res.getString("theme");
                Role role = Role.valueOf(res.getString("role"));

                User user = new User(id, role);
                ScoreStrategy strategy = new ScoreStrategyFromString().createStrategy(strategyName);
                user.setStrategy(strategy);
                user.setThemeStrategy(theme);
                users.add(user);
            }
            result.getValue().close();

        } catch (SQLException e) {
            System.out.println("Erreur lors de la recuperation des users : " + e);
        }

        return users;
    }

    @Override
    public User findOne(final String id) {
        String sql = "SELECT * FROM Users WHERE id = ?";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, id);
        ResultSet res = result.getKey();

        try {
            String userId = res.getString("id");
            String strategyName = res.getString("strategy");
            String theme = res.getString("theme");
            Role role = Role.valueOf(res.getString("role"));

            User user = new User(userId, role);
            ScoreStrategy strategy = new ScoreStrategyFromString().createStrategy(strategyName);
            user.setStrategy(strategy);
            user.setThemeStrategy(theme);

            result.getValue().close();
            return user;

        } catch (SQLException e) {
            System.out.println("Erreur lors de la recuperation du user :" + id);
        }
        return null;
    }

    @Override
    public void delete(final User user) {
        String sql = "DELETE FROM Users WHERE id = ?";
        QuerySql.query(sql, user.getId());
    }
}
