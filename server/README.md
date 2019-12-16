# server
The Imagic server that send responses to the Imagic client

## Prerequisites

In order to create and connect to DB, download MySQL Community Server 8 from:

https://dev.mysql.com/downloads/mysql/

Then, download MySQL Connector/C++ 8 from:

https://dev.mysql.com/downloads/connector/cpp/

## SQL command to create the DB

```
CREATE TABLE IF NOT EXISTS `mydb`.`image` (
  `idimage` INT NOT NULL AUTO_INCREMENT,
  `image_file_path` VARCHAR(45) NULL,
  `thumb_file_path` VARCHAR(45) NULL,
  `category` VARCHAR(45) NULL,
  PRIMARY KEY (`idimage`))
ENGINE = InnoDB
```
