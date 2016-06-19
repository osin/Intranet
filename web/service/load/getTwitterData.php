<?php
include '../../inc/init.inc';
//require 'config/functions.php';

if (!empty($_GET['oauth_verifier']) && !empty($_SESSION['oauth_token']) && !empty($_SESSION['oauth_token_secret'])) {
    // We've got everything we need
    $connector = Connector::find_by_name('Twitter');
    $twitteroauth = new TwitterOAuth($connector->api_key, $connector->api_secret, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
// Let's request the access token
    $access_token = $twitteroauth->getAccessToken($_GET['oauth_verifier']);
// Save it in a session var
    $_SESSION['access_token'] = $access_token;
// Let's get the user's info
    $user_info = $twitteroauth->get('account/verify_credentials');
// Print user's info
    echo '<pre>';
    print_r($user_info);
} else {
    echo 'oops';
    // Something's missing, go back to square 1
//    header('Location: login-twitter.php');
}
