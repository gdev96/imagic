CREATE DATABASE IF NOT EXISTS imagic;

CREATE USER IF NOT EXISTS
  'imagicuser'@'%' identified by 'ImgApp2020!';

GRANT ALL ON imagic.* TO 'imagicuser'@'%';

CREATE TABLE IF NOT EXISTS imagic.image (
  id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
  image_file_path VARCHAR(45) NULL,
  thumb_file_path VARCHAR(45) NULL,
  category VARCHAR(45) NULL
) ENGINE = InnoDB;
