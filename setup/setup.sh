#!/bin/bash

echo "Installing CMake..."
sudo snap install cmake --classic

# Install MySQL Server and MySQL Connector/C++

echo "Updating APT packages..."
sudo apt-get update

echo "Installing MySQL Server dependencies..."
sudo apt-get install -y libaio1

mkdir -p MySQLServer-8.0.19-1
cd MySQLServer-8.0.19-1

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

# Install Image Magick and delegate library dependencies

echo "Installing prerequisites..."
sudo apt-get install -y pkg-config

echo "Downloading ZLIB..."
curl http://www.imagemagick.org/download/delegates/zlib-1.2.11.tar.gz | tar xz

cd zlib-1.2.11

echo "Configuring ZLIB..."
./configure

echo "Compiling ZLIB..."
make

echo "Installing ZLIB..."
sudo make install

cd ..

echo "Downloading JPEG delegate..."
curl http://www.imagemagick.org/download/delegates/jpegsrc.v9b.tar.gz | tar xz

cd jpeg-9b

echo "Configuring JPEG delegate..."
./configure

echo "Compiling JPEG delegate..."
make

echo "Installing JPEG delegate..."
sudo make install

cd ..

echo "Downloading PNG delegate..."
curl http://www.imagemagick.org/download/delegates/libpng-1.6.31.tar.gz | tar xz

cd libpng-1.6.31

echo "Configuring PNG delegate..."
./configure

echo "Compiling PNG delegate..."
make

echo "Installing PNG delegate..."
sudo make install

cd ..

echo "Downloading TIFF delegate..."
curl http://www.imagemagick.org/download/delegates/tiff-4.0.8.tar.gz | tar xz

cd tiff-4.0.8

echo "Configuring TIFF delegate..."
./configure

echo "Compiling TIFF delegate..."
make

echo "Installing TIFF delegate..."
sudo make install

cd ..

echo "Downloading Image Magick..."
curl https://imagemagick.org/download/releases/ImageMagick-7.0.9-27.tar.gz | tar xz

cd ImageMagick-7.0.9-27

echo "Configuring Image Magick..."
./configure

echo "Compiling Image Magick..."
make -j$(nproc)

echo "Installing Image Magick..."
sudo make install

cd ../..

# Install client libraries

echo "Installing PyQt5..."
pip3 install -r client/requirements.txt
