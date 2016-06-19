<?php

include '../../inc/init.inc';
if (isset($res->user))
  $res->load('timeline');
isset($signin) ? $result = "Veuillez vous inscrire ou vous connecter pour accèder à toutes les fonctionnalités du site" : $result = "";
if (isset($login) && isset($password)) {
    $users = User::find('all', array('conditions' => array('email' => strtolower($login), 'password' => sha1($password . PWD_HASH))));
  if ($users->count > 0) {
    $user = $users->first();
    session_name('Wotoog');
    session_start();
    $user->last_connection = date("Y-m-d H:i:s", time());
    $user->save();
    $_SESSION['Wotoog'] = $user;
    $res->load(isset($redirect) ? $redirect : 'timeline', isset($args) ? $args : null);
  }
  else
    $result = "Vérifier les informations que vous avez entrées";
}
$res->useTemplate('Bienvenue', $result);