<?php
  //les variables comme posts et tests ne sont pas transmise,
  $res->notificationsNumber = Notification::all(array('conditions' => array('to_user_id' => $res->user->id, 'status' => 0)))->count;
  $res->useTemplate();
?>
