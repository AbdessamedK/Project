package fr.univ_lyon1.info.m1.microblog.model.strategy;

import java.lang.reflect.InvocationTargetException;

/**
 * Class that defines a method to create an instance of a ScoreStrategy.
 */
public class ScoreStrategyFromString {
    private static final String PACKAGE = "fr.univ_lyon1.info.m1.microblog.model.strategy.";

    /**
     * Create and return an instance of the strategy specified by name.
     * @param strategyName the strategy to create in String.
     * @return the instance create.
     */
    public ScoreStrategy createStrategy(final String strategyName) {
        try {
            
            String className = PACKAGE + strategyName;
            Class<?> strategy = Class.forName(className);
            return (ScoreStrategy) strategy.getDeclaredMethod("getInstance").invoke(null);

        } catch (ClassNotFoundException e) {
            System.out.println("Classe non trouvee : " + e);
        } catch (NoSuchMethodException e) {
            System.out.println("Methode getInstance() non trouvee" + e);
        } catch (IllegalAccessException | SecurityException | InvocationTargetException e) {
            System.out.println("Erreur : " + e);
        }
        return null;
    }
}
