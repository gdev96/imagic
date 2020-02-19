INSTALL PLUGIN auth_socket SONAME 'auth_socket.so';
UPDATE mysql.user SET plugin = 'auth_socket' WHERE User = 'root';
