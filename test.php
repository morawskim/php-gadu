<?php

#change settings below!
define('GG_UID', 00000000);
define('GG_PASSWORD', 'password');
define('GG_SEND_TO', 00000000);
$messageToSend = 'Hello from php gadu gadu library!';


if (!extension_loaded('gadu')) {
    fwrite(STDERR, 'Gadu extension\'s not installed');
    exit(1);
}

$session = gadu_login(GG_UID, GG_PASSWORD);
if (!$session) {
    fwrite(STDERR, 'Cant login. Probably wrong username/password');
    unset($session);
    exit(1);
}

gadu_notify($session);
if (!$session) {
    fwrite(STDERR, 'Cant notify gg server. Probably connection was closed');
    unset($session);
    exit(1);
}

gadu_send_message($session, GG_SEND_TO, $messageToSend);
if (!$session) {
    fwrite(STDERR, 'Cant send gg message to user');
    unset($session);
    exit(1);
}

gadu_logoff($session);

