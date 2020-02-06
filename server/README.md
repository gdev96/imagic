# server
The Imagic server that send responses to the Imagic client.

## Prerequisites

### MySQL

In order to create and connect to DB, download MySQL Community Server 8 from:

https://dev.mysql.com/downloads/mysql/

Install it following the guide (for Debian GNU/Linux only):

https://dev.mysql.com/doc/mysql-installation-excerpt/8.0/en/linux-installation-debian.html

Then, download the MySQL Connector/C++ 8, implementing the X DevAPI, from:

https://dev.mysql.com/downloads/connector/cpp/

and make a complete installation following the guide:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-installation-binary.html

Be sure to download the development files required for building the application.

For further details, see:

https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-apps-general-considerations.html

### Image Magick

In order to create the image thumbnails, you need to download Imagic Magick.

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

https://imagemagick.org/download/ImageMagick.tar.gz

Now, you need to repeat the steps above.

Unpack the distribution with this command:

```
tar xvzf ImageMagick.tar.gz
```

Next configure and compile ImageMagick:

```
cd ImageMagick-7.0.9-21
./configure
make
```

Administrator privileges are required to install. To install, type:

```
sudo make install
```

## Build problems

If build fails, edit `CMakeLists.txt` file and verify that:

1. correct header files are included
2. correct libraries are linked
