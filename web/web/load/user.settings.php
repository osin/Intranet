<?php
include '../../inc/init.inc';
if(!isset($infos))
  $infos = 'Ici vous pouvez changer vos informations de compte';
if (!empty($_GET['oauth_verifier']) && !empty($_SESSION['oauth_token']) && !empty($_SESSION['oauth_token_secret'])) {
    $connector = Connector::find_by_name('Twitter');
    $twitteroauth = new TwitterOAuth($connector->api_key, $connector->api_secret, $_SESSION['oauth_token'], $_SESSION['oauth_token_secret']);
    $access_token = $twitteroauth->getAccessToken($_GET['oauth_verifier']);
    $_SESSION['access_token'] = $access_token;
    $res->twitterUser = $twitteroauth->get('account/verify_credentials');
}
    $res->useTemplate();
?>
