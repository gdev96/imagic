# server
The Imagic server that send responses to the Imagic client.

## Prerequisites

In order to create and connect to DB, download MySQL Community Server 8 from:

https://dev.mysql.com/downloads/mysql/

Install it following the guide (for Debian GNU/Linux only):

https://dev.mysql.com/doc/mysql-installation-excerpt/8.0/en/linux-installation-debian.html

Then, download the MySQL Connector/C++ 8, implementing the X DevAPI, from:

https://dev.mysql.com/downloads/connector/cpp/

and make a complete installation following the guide:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-installation-binary.html

Be sure to download the development files required for building the application.
If build fails, edit `CMakeLists.txt` file and verify that:

1. correct header files are included
2. correct libraries are linked

For further details, see:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-apps-general-considerations.html

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
