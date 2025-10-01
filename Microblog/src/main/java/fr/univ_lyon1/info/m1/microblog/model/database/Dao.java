package fr.univ_lyon1.info.m1.microblog.model.database;

import java.util.Collection;

/**
 * Generic DAO Interface.
 * @param <T> Type of objects to be supported by a DAO.
 * @param <KEY> Type of the key to identify entities.
 * 
 * @author Lionel Medini
 */
public interface Dao<KEY, T> {
    /**
    * Adds an element.
    * @param element The element to add
    */
    void add(T element);

    /**
    * Returns all elements.
    * @return The (potentially empty) collection of stored elements
    */
    Collection<T> findAll();

    /**
    * Updates an element or creates it if it doesn't exist.
    * @param id The key of the element to update
    * @param element The element to replace the old element with
    */
    void update(KEY id, T element);

    /**
    * Returns an element from its key.
    * @param id The key of the element being searched for
    * @return The element whose key is the one passed as a parameter
    */
    T findOne(KEY id);

    /**
    * Removes an element.
    * @param element The element to remove
    */
    void delete(T element);
}
