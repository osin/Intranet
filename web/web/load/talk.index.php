<?php
$res->talkMembres = $res->user->getFriends()->asArray();
$res->useTemplate();
?>