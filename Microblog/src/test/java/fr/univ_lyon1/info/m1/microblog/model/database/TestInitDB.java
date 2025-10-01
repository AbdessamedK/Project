package fr.univ_lyon1.info.m1.microblog.model.database;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import javafx.util.Pair;

/** 
 * Test if the database contains all tables 
 * and that the tables contain the correct number of columns.
 */
public final class TestInitDB {
    
    @BeforeAll
    @SuppressWarnings("unused")
    static void setUp() {
        new InitDB();
    }

    private boolean existsTable(final String nameTable) {
        String sql = "SELECT name FROM sqlite_master WHERE"
            + " type = 'table' AND name = ?";
        
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql, nameTable);
        ResultSet res = result.getKey();

        try {
            if (res.next()) {
                result.getValue().close();
                return true;
            }  
        } catch (SQLException e) {
            System.out.println("Erreur existsTable !");
        }
        return false;
    }

    private int getColumns(final String nameTable) {
        String sql = "PRAGMA table_info(" + nameTable + ")";
        Pair<ResultSet, Connection> result = QuerySql.querySelect(sql);
        ResultSet res = result.getKey();
        int nb = 0;

        try {
            while (res.next()) {
                nb++;
            }

        } catch (SQLException e) {
            System.out.println("Erreur getColumns ! ");
        }

        return nb;
    }

    @Test 
    void testExists() {
        assertTrue(existsTable("Users"));
        assertTrue(existsTable("Message"));
        assertTrue(existsTable("MessageData"));
    }

    @Test 
    void testColumns() {
        assertEquals(4, getColumns("Users"));
        assertEquals(4, getColumns("Message"));
        assertEquals(5, getColumns("MessageData"));
    }
    
}
