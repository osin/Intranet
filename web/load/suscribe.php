<?php
include '../../inc/init.inc';
$result = "Veuillez remplir toutes les informations demandées";
if (isset($captcha) && isset($vcaptcha)) {
    if (md5($captcha) == $vcaptcha) {
        //@TODO : check si le login passé est numeric ou non
        if ($password != "" && $repassword != "" && $email != "" && $firstname != "" && $lastname != "") {
            if (!isset($picture))
                $picture = "";
            if ($password != $repassword) {
                $result = array("error" => "Les mots de passe ne concordent pas");
            } elseif (User::exists(array('conditions' => array('email' => $email))) > 0) {
                $result = array("error" => "Ce compte existe déjà");
            } else {
                $user = User::create(array(
                    'password'  => sha1($password . PWD_HASH),
                    'email'     => $email,
                    'firstname' => ucfirst(strtolower($firstname)),
                    'lastname'  => ucfirst(strtolower($lastname)),
                    'picture'   => $picture,
                ));
                session_name('Wotoog');
                session_start();
                $user->last_connection = date("Y-m-d H:i:s", time());
                $user->save();
                $_SESSION['Wotoog'] = $user;
                $res->load("membres", array("new_member" => TRUE));
            }
        } else
            $result = array("error" => "Formulaire incomplet");
    } else {
        $result = array("error" => "Erreurs captcha");
    }
}
$res->nb1 = rand(1, 5);
$res->nb2 = rand(1, 5);
$res->somme = $res->nb1 + $res->nb2;
$res->captcha_crypted = md5($res->somme);
$res->useTemplate('Inscription', $result);