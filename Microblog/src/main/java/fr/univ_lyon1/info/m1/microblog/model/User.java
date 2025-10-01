package fr.univ_lyon1.info.m1.microblog.model;

import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;

/**
 * User of the application.
 */
public class User {
    private String id;
    private ScoreStrategy strategy;
    private String themeStrat;
    private Role role;

    @Override
    public int hashCode() {
        return id.hashCode();
    }

    @Override
    public boolean equals(final Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        User other = (User) obj;
        if (id == null) {
            if (other.id != null) {
                return false;
            }
        } else if (!id.equals(other.id)) {
            return false;
        }
        return true;
    }

    /**
     * Default constructor for User.
     * @param id must be a unique identifier.
     */
    public User(final String id, final Role role) {
        this.id = id;
        this.role = role;
    }

    public ScoreStrategy getStrategy() {
        return strategy;
    }

    public void setStrategy(final ScoreStrategy strategy) {
        this.strategy = strategy;
    }
    
    public String getThemeStrategy() {
        return this.themeStrat;
    }

    public void setThemeStrategy(final String themeStrate) {
        this.themeStrat = themeStrate;
    }

    public String getId() {
        return id;
    }

    public void setId(final String id) {
        this.id = id;
    }

    public Role getRole() {
        return role;
    }

    public void setRole(final Role role) {
        this.role = role;
    }
}
