# imagic
A platform for uploading and downloading images

//SQL COMMANDS TO CREATE THE SUPPORT DB

CREATE TABLE IF NOT EXISTS `mydb`.`image` (
  `idimage` INT NOT NULL AUTO_INCREMENT,
  `image_file_path` VARCHAR(45) NULL,
  `thumb_file_path` VARCHAR(45) NULL,
  `category` VARCHAR(45) NULL,
  PRIMARY KEY (`idimage`))
ENGINE = InnoDB

