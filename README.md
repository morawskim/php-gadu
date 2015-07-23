INSTALATION
=======

You must have libgadu and libgadu-devel package installed.

Download source code, unzip and go to php-gadu dir.

    wget https://github.com/morawskim/php-gadu/archive/master.zip
    unzip ./master.zip
    cd ./php-gadu-master/
    
Run phpize (You must have installed php-devel package)

    phpize
    
Run configure script

    ./configure --enable-gadu
    
Compile

    make
    
Copy extension
    
    php -i | grep -i extension_dir
    cp -v ./modules/gadu.so /path/where/php/extensions/are/stored/see/above/command/result
    
Enable extension To see where is php.ini file run below command.
 
    php -i | grep ini

Add below line to php.ini. Or create new ini file in additional ini file dir.

    extension=gadu.so

Run and check.

    php -m | grep gadu
    
You should see

    gadu
    
