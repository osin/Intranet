<?php
include '../../inc/init.inc';
try{
$user = User::find($res->user->id);
if ($email != "")
  $user->email = $email;
if ($password != "" && $password == $repassword) {
  $user->password = sha1($password.PWD_HASH);
}
if ($picture != "") {
  $user->picture = $picture;
}
$user->save();
$_SESSION['lbi_user'] = $user;
$res->user = $user;
$res->load('user.settings', array('infos' => array ('success' => 'Informations mise à jour avec succès')));
}catch(Exception $e){
  $res->load('user.settings', array('infos' => array ('error' => "Une erreur s'est produite, il se peut que vous ne puissiez plus accéder à votre compte, veuillez contacter l'administrateur du site")));
}

?>
