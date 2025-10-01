package fr.univ_lyon1.info.m1.microblog.model;

import static org.junit.Assert.assertEquals;

import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategy;
import fr.univ_lyon1.info.m1.microblog.model.strategy.ScoreStrategyFromString;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;



/**
 *  Test the message of my application.
 */
public final class TestUser {

        private User u1;
        private User u2;
        private User u3;
        private ScoreStrategy s1;
        private ScoreStrategy s2;
        private ScoreStrategy s3;



        /**
         *  Function to setup messages.
         */
        @BeforeEach
        public void setUp() {
                u1 = new User("Amir", Role.ADMIN);
                u2 = new User("", Role.USER);
                u3 = new User("Abde", Role.ADMIN);

                u2.setId("Homme");

                s1 = new ScoreStrategyFromString().createStrategy(
                "RevelantRecentPostStrategy");
                s2 = new ScoreStrategyFromString().createStrategy(
                "RevelantPostStrategy");
                s3 = new ScoreStrategyFromString().createStrategy(
                "RevelantPostStrategy");

                u1.setStrategy(s1);
                u2.setStrategy(s2);
                u3.setStrategy(s3);
        }

        /**
         *  Function to test Name of the User.
         */
        @Test
        public void testNameUser() {
                assertEquals("Amir", u1.getId());
                assertEquals("Homme", u2.getId());
                assertEquals("Abde", u3.getId());
        }
        /**
         *  Function to test Role of the User.
         */
        @Test
        public void testRoleUser() {
                assertEquals(Role.ADMIN, u1.getRole());
                assertEquals(Role.USER, u2.getRole());
                assertEquals(Role.ADMIN, u3.getRole());
        }
        
        /**
        *  Function to test Strategie of User.
        */
        @Test
        public void testStategieUser() {
                assertEquals(s1, u1.getStrategy());
                assertEquals(s2, u2.getStrategy());
                assertEquals(s3, u3.getStrategy());
        }

        
}
