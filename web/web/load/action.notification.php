<?php
include '../../inc/init.inc';
if (isset($notification)){
  $notification = Notification::find($notification);
  if (isset($type) && $type == "updateStatus") {
    if ($notification->to_user_id != $res->user->id) {
      throw new Exception("user is not autorized", 504, $res->user->id);
    }
    $notification->status = 1;
    $notification->save();
  }
}
isset($redirect) ? $res->load($notification->action) : "";

?>