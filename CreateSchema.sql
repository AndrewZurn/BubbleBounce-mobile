DROP SCHEMA IF EXISTS BOUNCIN_BALLS
CREATE SCHEMA BOUNCIN_BALLS

USE BOUNCIN_BALLS

CREATE TABLE USERS(
  USER_ID		INTEGER		AUTO_INCREMENT,
  DISPLAY_NAME	VARCHAR(15)	NOT NULL,
  
  PRIMARY KEY(USER_ID)
);

CREATE TABLE SCORES(
  SCORE_ID		INTEGER		AUTO_INCREMENT,
  VALUE			INTEGER		NOT NULL,
  TIME			TIMESTAMP	NOT NULL,
  USER_ID		INTEGER		NOT NULL,
  
  PRIMARY KEY(SCORE_ID),
  FOREIGN KEY USER_ID REFERENCES USERS(USER_ID)
    ON UPDATE CASCADE ON DELETE CASCADE
);
  