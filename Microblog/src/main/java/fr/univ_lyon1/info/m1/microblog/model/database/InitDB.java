package fr.univ_lyon1.info.m1.microblog.model.database;

/**
 * Class that creates all tables if they do not exist in the database.
 */
public class InitDB {
    /** Base builder. */
    public InitDB() {        
        createUserTable();
        createMessageTable();
        createMessageDataTable();
    }

    private void createUserTable() {
        String sql = "CREATE TABLE IF NOT EXISTS Users (" 
            + "id TEXT PRIMARY KEY,"
            + "strategy TEXT,"
            + "theme TEXT,"
            + "role TEXT)";
        QuerySql.query(sql);
    }

    private void createMessageTable() {
        String sql = "CREATE TABLE IF NOT EXISTS Message ("
            + "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            + "owner_id TEXT,"
            + "content TEXT," 
            + "date TEXT,"
            + "FOREIGN KEY (owner_id) REFERENCES Users(id) ON DELETE CASCADE)";
        QuerySql.query(sql);
    }

    private void createMessageDataTable() {
        String sql = "CREATE TABLE IF NOT EXISTS MessageData (" 
            + "user_id TEXT," 
            + "message_id TEXT," 
            + "bookmarked TEXT," 
            + "score TEXT," 
            + "words TEXT," 
            + "PRIMARY KEY (user_id, message_id)," 
            + "FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE," 
            + "FOREIGN KEY (message_id) REFERENCES Message(id) ON DELETE CASCADE)";
        QuerySql.query(sql);
    }
}
