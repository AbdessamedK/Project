package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javafx.util.Pair;

/**
 * Class to execute a query on the database.
 */
public final class QuerySql {
    private static final String URL = "jdbc:sqlite:src/main/resources/database.db";
    
    private QuerySql() { }

    /**
     * Function that executes a SQL Select query and returns the result.
     * @param sql The sql query.
     * @param params The parameters required to execute the query.
     * @return A pair of the ResultSet (the result of the query)and the 
     * connection to be able to close it properly after using the result.
     */
    public static Pair<ResultSet, Connection> querySelect(
        final String sql, final String... params) {
        
        Connection connection = null;
        ResultSet res = null;
        
        try {
            Class.forName("org.sqlite.JDBC");
            connection = DriverManager.getConnection(URL);

            if (params.length == 0) {
                Statement stmt = connection.createStatement();
                res = stmt.executeQuery(sql);
            } else {
                PreparedStatement stmt = connection.prepareStatement(sql);
                for (int i = 0; i < params.length; i++) {
                    stmt.setString(i + 1, params[i]);
                }
                res = stmt.executeQuery();
            }

        } catch (ClassNotFoundException | SQLException e) {
            System.out.println("Erreur : " + e);
        }

        return new Pair<>(res, connection);
    }

    /**
     * Function to execute the command in parameter.
     * @param sql The sql query.
     * @param params The parameters required to execute the query.
     */
    public static void query(final String sql, final String... params) {
        Connection connection;

        try {
            Class.forName("org.sqlite.JDBC");
            connection = DriverManager.getConnection(URL);

            Statement pragmaStmt = connection.createStatement();
            pragmaStmt.executeUpdate("PRAGMA foreign_keys = ON;");
            pragmaStmt.close();

            if (params.length == 0) {
                Statement stmt = connection.createStatement();
                stmt.executeUpdate(sql);
            } else {
                PreparedStatement stmt = connection.prepareStatement(sql);
                for (int i = 0; i < params.length; i++) {
                    stmt.setString(i + 1, params[i]);
                }
                stmt.executeUpdate();
            }

            connection.close();
            
        } catch (ClassNotFoundException | SQLException e) {
            System.out.println("Erreur : " + e);
        }
    }
}
