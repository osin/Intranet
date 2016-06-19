<?php
//include '../../inc/init.inc';
$res->followers = $res->membre->getPotientialUserNetwork()->asArray();
$res->useTemplate("");
?>