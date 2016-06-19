<?php
$res->notifications = Notification::all(array('conditions' => array('to_user_id' => $res->user->id, 'status' => 0), 'limit' => 5))->asArray();
$res->useTemplate();
?>