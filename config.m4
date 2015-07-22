PHP_ARG_ENABLE(gadu,
	[Whether to enable the "gadu" extension],
	[	-enable-gadu	Enable "gadu" extension support])

if test $PHP_GADU != "no"; then

    #QR_ENCODED=/home/marcin/Projekty/libqrencode/build/
    #PHP_ADD_INCLUDE($QR_ENCODED/include)
    #PHP_ADD_LIBRARY_WITH_PATH(qrencode, $QR_ENCODED/lib64, GADU_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(gadu, /usr/lib64, GADU_SHARED_LIBADD)

    PHP_SUBST(GADU_SHARED_LIBADD)
    PHP_NEW_EXTENSION(gadu, gadu.c, $ext_shared)

fi




