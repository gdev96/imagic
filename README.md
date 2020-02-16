# Imagic

A platform for uploading and downloading images.

## Architecture

The application architecture is client-server.
There are:

- one or more clients that make requests
- a load balancer (frontend) that forwards requests to the most unloaded servers
- one or more servers that handle client requests

<img width="600" src="https://github.com/gdev96/imagic/blob/master/resources/imagic_schema.svg">

## Message format

Messages exchanged by client and servers have the following fixed format:

<img width="600" src="https://github.com/gdev96/imagic/blob/master/resources/message_format.svg">

## Setup

There are two options to start the application, depending on the platform you are running.

### Ubuntu 18.04

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

##### CMake

You have to install CMake (version 3.15 or higher).

##### MySQL

In order to create and connect to DB, download MySQL Community Server 8 from:

https://dev.mysql.com/downloads/mysql/

Install it following the guide:

https://dev.mysql.com/doc/mysql-installation-excerpt/8.0/en/

Then, download the MySQL Connector/C++ 8, implementing the X DevAPI, from:

https://dev.mysql.com/downloads/connector/cpp/

and make a complete installation following the guide:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-installation-binary.html

Be sure to download the development files required for building the application.

##### Image Magick

In order to create the image thumbnails, you need to download and install Imagic Magick.

First, download JPEG delegate from:

http://www.imagemagick.org/download/delegates/jpegsrc.v9b.tar.gz

Unpack the distribution with this command:

```
tar xvzf jpegsrc.v9b.tar.gz
```

Next configure and compile ImageMagick:

```
cd jpeg-9b
./configure
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
./configure
make
```

Administrator privileges are required to install. To install, type:

```
sudo make install
```

For further details, see:

https://imagemagick.org/script/download.php

#### Build problems

If build fails, edit `CMakeLists.txt` file and verify that:

1. correct header files are included
2. correct libraries are linked

In particular, for MySQL you can see:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-apps-general-considerations.html
