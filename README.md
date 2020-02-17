# Imagic

A platform for uploading and downloading images.

## Architecture

The application architecture is client-server.

There are:

- one or more clients that make requests
- a load balancer (frontend) that forwards requests to the most unloaded servers
- one or more servers that handle client requests

<img width="600" src="https://github.com/gdev96/imagic/blob/master/resources/imagic_schema.svg">

Every server has its own database which collects infos about images.

## Message format

Messages exchanged by client and servers have the following fixed format:

<img width="900" src="https://github.com/gdev96/imagic/blob/master/resources/message_format.svg">

## Setup

There are two options to start the application, depending on the platform you are running.

### Ubuntu 18.04 (*recommended*)

To run the application move to the `setup` directory, placed in the root of the project, and execute, in order:

1. the `setup.sh` script;
2. the `start.sh` script.

Just type in terminal:

```
cd setup
./setup.sh

...

./start.sh
```

### Other platforms

#### Prerequisites

##### 1. CMake 3.15 (or higher)

You have to install CMake 3.15 (or higher) in order to build the load balancer and the server projects using the `CMakeLists.txt` files.

##### 2. MySQL Server 8

In order to create and connect to DB, server needs MySQL Community Server 8.

Download it from:

https://dev.mysql.com/downloads/mysql/

Install it following the guide:

https://dev.mysql.com/doc/mysql-installation-excerpt/8.0/en/

Then, download the MySQL Connector/C++ 8, implementing the X DevAPI, from:

https://dev.mysql.com/downloads/connector/cpp/

and make a complete installation following the guide:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-installation-binary.html

Be sure to download the development files required for building the application.

##### 3. Image Magick 7

In order to create the image thumbnails, server needs Imagic Magick 7.

Here are the instructions to download and install it.

To work with a specific image format you need to install the right delegate.

For example, to work with JPEG images, download the following:

http://www.imagemagick.org/download/delegates/jpegsrc.v9b.tar.gz

Unpack the distribution with this command:

```
tar xvzf jpegsrc.v9b.tar.gz
```

Next configure and compile ImageMagick:

```
cd jpeg-9b
./configure --prefix=/usr
make
```

Administrator privileges are required to install. To install, type:

```
sudo make install
```

Then, download Image Magick sources from:

https://imagemagick.org/download/releases/ImageMagick-7.0.9-23.tar.gz

Now, you need to repeat the steps above.

Unpack the distribution with this command:

```
tar xvzf ImageMagick-7.0.9-23.tar.gz
```

Next configure and compile ImageMagick:

```
cd ImageMagick-7.0.9-23
./configure --prefix=/usr
make
```

Administrator privileges are required to install. To install, type:

```
sudo make install
```

For further details, see:

https://imagemagick.org/script/download.php

##### 4. PyQt5

Finally, you have to install PyQt5 in order to start the client.

Simply type, from project root:

```
pip3 install -r client/requirements.txt
```

### Troubleshooting

#### Build problems

If build fails, edit `CMakeLists.txt` file in server project and verify that:

1. correct header files are included
2. correct libraries are linked

In particular, for MySQL you can see:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-apps-general-considerations.html

#### MySQL authentication problems

If databases creation fails, maybe you need to install the `auth_socket` plugin.

Open a terminal and type:

```
mysql -u root -p
INSTALL PLUGIN auth_socket SONAME 'auth_socket.so';
UPDATE mysql.user SET plugin = 'auth_socket' WHERE User = 'root';
```
