<?php
include '../../inc/init.inc';
$connector = Connector::find_by_name('Twitter');
$facebook = new Facebook(array(
            'appId' => $connector->api_key,
            'secret' => $connector->api_secret,
            ));

$user = $facebook->getUser();

if ($user) {
  try {
    // Proceed knowing you have a logged in user who's authenticated.
    $user_profile = $facebook->api('/me');
  } catch (FacebookApiException $e) {
    error_log($e);
    $user = null;
  }






    if (!empty($user_profile )) {
        echo '<pre>' . print_r($user_profile, true) . '</pre>';
    }
} else {
    # There's no active session, let's generate one
	$login_url = $facebook->getLoginUrl(array( 'scope' => 'email'));
    header("Location: " . $login_url);
}
?>
