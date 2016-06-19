<?php
include '../../inc/init.inc';
  $res= $user = null;
  session_unset();
  session_destroy();
  $res = Template::instance('template');
  $res->load("index", array("signin" => 1));
?>