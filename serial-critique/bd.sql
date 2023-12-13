/*
** Jeu de données Series
** UCB Lyon 1 - BDW1 - Fabien Duchateau - 2021
*/

/* Dropping existing tables */
DROP TABLE IF EXISTS Critiques;
DROP TABLE IF EXISTS Jouer;
DROP TABLE IF EXISTS Actrices; 
DROP TABLE IF EXISTS Episodes; 
DROP TABLE IF EXISTS Saisons; 
DROP TABLE IF EXISTS Series;

/* Creating tables */ 

CREATE TABLE Series ( nomSerie VARCHAR(255), PRIMARY KEY(nomSerie)); 
CREATE TABLE Saisons ( idSaison INTEGER,
dateLancement DATE, nomSerie VARCHAR(255) REFERENCES Series(nomSerie), PRIMARY KEY(idSaison)); 
CREATE TABLE Episodes ( idSaison INTEGER REFERENCES Saisons(idSaison), numero INTEGER, titre VARCHAR(255), PRIMARY KEY(numero, idSaison)); 
CREATE TABLE Actrices ( numINSEE INTEGER, nom VARCHAR(100), prenom VARCHAR(100), PRIMARY KEY(numINSEE)); 
CREATE TABLE Jouer ( idSaison INTEGER REFERENCES Saisons(idSaison), numero INTEGER REFERENCES Episodes(numero), numINSEE INTEGER REFERENCES Actrices(numINSEE), salaire DOUBLE, PRIMARY KEY(idSaison, numero, numINSEE)); 
CREATE TABLE Critiques ( idCritique INTEGER NOT NULL AUTO_INCREMENT, dateCritique DATETIME, pseudo VARCHAR(100), texte VARCHAR(255), nomSerie VARCHAR(255) REFERENCES Series(nomSerie), PRIMARY KEY(idCritique));

/* Inserting instances */ 
 
INSERT INTO Series VALUES('The Big Bang Theory');
INSERT INTO Series VALUES('Game of Thrones');
INSERT INTO Series VALUES('Breaking Bad');
INSERT INTO Series VALUES('The Wire');
INSERT INTO Series VALUES('Black Clover');
INSERT INTO Series VALUES('The 100');
INSERT INTO Series VALUES('Kaamelott');
INSERT INTO Saisons VALUES(1, '2011-09-22', 'The Big Bang Theory');
INSERT INTO Saisons VALUES(2, '2012-09-27', 'The Big Bang Theory');
INSERT INTO Saisons VALUES(3, '2011-04-17', 'Game of Thrones');
INSERT INTO Saisons VALUES(4, '2014-03-19', 'The 100');
INSERT INTO Saisons VALUES(5, '2005-03-01', 'Kaamelott');
INSERT INTO Episodes VALUES(1, 1, 'The Skank Reflex Analysis');
INSERT INTO Episodes VALUES(2, 1, 'The Date Night Variable');
INSERT INTO Episodes VALUES(3, 1, 'Winter is coming');
INSERT INTO Episodes VALUES(3, 2, 'The Kingsroad');
INSERT INTO Episodes VALUES(4, 1, 'Pilot');
INSERT INTO Episodes VALUES(5, 3, 'La Table de Breccan');
INSERT INTO Actrices VALUES(111, 'Bean', 'Sean');
INSERT INTO Actrices VALUES(222, 'Fairley', 'Michelle');
INSERT INTO Actrices VALUES(333, 'Cuoco', 'Kaley');
INSERT INTO Actrices VALUES(444, 'Parsons', 'Jims');
INSERT INTO Actrices VALUES(555, 'Avgeropoulos', 'Marie');
INSERT INTO Jouer VALUES(1, 1, 333, 3200);
INSERT INTO Jouer VALUES(1, 1, 444, 3200);
INSERT INTO Jouer VALUES(2, 1, 333, 3200);
INSERT INTO Jouer VALUES(3, 1, 111, 8437);
INSERT INTO Jouer VALUES(3, 1, 222, 6000);
INSERT INTO Jouer VALUES(3, 2, 111, NULL);
INSERT INTO Jouer VALUES(3, 2, 222, 6000);
INSERT INTO Critiques VALUES(1, '2012-02-05 22:03:54', 'user12345', 'Une super série !', 'The Big Bang Theory');
INSERT INTO Critiques VALUES(2, '2016-11-25 15:42:06', 'welshman', 'j kiff tro daeneris !!!!! :) 8)', 'Game of Thrones');
INSERT INTO Critiques VALUES(3, '2019-02-02 21:33:36', 'fan2100', 'vivement la prochaine saison !', 'The 100');
INSERT INTO Critiques VALUES(4, '2020-12-27 12:43:01', 'user12345', 'Encore 6 mois avant le film !', 'Kaamelott');

