#!/bin/bash

echo "Installing CMake..."
sudo snap install cmake --classic

# Install MySQL Server and MySQL Connector/C++
echo "Updating APT packages..."
sudo apt-get update

echo "Installing MySQL Server dependencies..."
sudo apt-get install libaio1

mkdir -p MySQL-Server-8.0.19
cd MySQL-Server-8.0.19

echo "Downloading MySQL Server..."
curl https://cdn.mysql.com/Downloads/MySQL-8.0/mysql-server_8.0.19-1ubuntu18.04_amd64.deb-bundle.tar | tar x

echo "Configuring MySQL Server..."
sudo dpkg-preconfigure mysql-community-server_*.deb

echo "Installing MySQL Server..."
sudo dpkg -i mysql-{common,community-client-core,community-client,client,community-server-core,community-server,server}_*.deb

echo "Resolving dependencies of MySQL Server..."
sudo apt-get -f -y install

echo "Installing auth_socket plugin..."
mysql -u root -p < ../setup.sql

echo "Restarting MySQL Server..."
sudo service mysql restart

echo "Downloading MySQL Connector/C++ files..."
curl --remote-name-all \
https://cdn.mysql.com/Downloads/Connector-C++/libmysqlcppconn7_8.0.19-1ubuntu18.04_amd64.deb \
https://cdn.mysql.com/Downloads/Connector-C++/libmysqlcppconn8-2_8.0.19-1ubuntu18.04_amd64.deb \
https://cdn.mysql.com/Downloads/Connector-C++/libmysqlcppconn-dev_8.0.19-1ubuntu18.04_amd64.deb

echo "Installing MySQL Connector/C++..."
sudo dpkg -i libmysqlcppconn*.deb

echo "Resolving dependencies of MySQL Connector/C++..."
sudo apt-get -f -y install

cd ..

# Install Image Magick and JPEG delegate
echo "Downloading JPEG delegate..."
curl http://www.imagemagick.org/download/delegates/jpegsrc.v9b.tar.gz | tar xz

cd jpeg-9b

echo "Configuring JPEG delegate..."
./configure --prefix=/usr

echo "Compiling JPEG delegate..."
make

echo "Installing JPEG delegate..."
sudo make install

cd ..

echo "Downloading Image Magick..."
curl https://imagemagick.org/download/releases/ImageMagick-7.0.9-24.tar.gz | tar xz

cd ImageMagick-7.0.9-24

echo "Configuring Image Magick..."
./configure --prefix=/usr

echo "Compiling Image Magick..."
make

echo "Installing Image Magick..."
sudo make install

cd ../..

echo "Installing PyQt5..."
pip3 install -r client/requirements.txt
