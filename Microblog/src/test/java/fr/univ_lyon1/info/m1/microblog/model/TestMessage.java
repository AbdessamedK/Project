package fr.univ_lyon1.info.m1.microblog.model;

import static org.junit.Assert.assertEquals;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;



/**
 *  Test the message of my application.
 */
public final class TestMessage {

        private Message m1;
        private Message m2;
        private Message m3;

        /**
         *  Function to setup messages.
         */
        @BeforeEach
        public void setUp() {
                User temp = new User("Test% toto", Role.USER);
                m1 = new Message("", temp);
                m2 = new Message("Un message pas vide", temp);
                m3 = new Message("ABDCS", temp);
                m3.setId(0);
        }

        /**
         *  Function to test Normal messages.
         */
        @Test
        public void testNormalString() {
                assertEquals("Un message pas vide", m2.getContent());
                assertEquals("ABDCS", m3.getContent());
                assertEquals(0, m3.getId());
        }

        /**
         *  Function to test Empty messages.
         */
        @Test
        public void testEmptyString() {
                assertEquals("", m1.getContent());
        }

}
