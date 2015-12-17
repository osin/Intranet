<?php
if ($res->user->id != $res->membre->id) {
  throw new Exception("Not Authorized action");
}
if (isset($referer_id)) {
  $res->membreMail = User::find($referer_id);
  //$res->mails = Mail::all(array('conditions' => array('referer_id' => $res->user->id, 'user_id' => $referer_id)))->asArray();
  $res->mails = Mail::all(array('conditions' => array('(referer_id = ? OR referer_id = ?) AND (user_id = ? OR user_id = ?)', $referer_id, $res->user->id, $res->user->id, $referer_id)))->asArray();
  foreach (Mail::all(array('conditions' => array('referer_id' => $res->user->id, 'user_id' => $referer_id)))->asArray() as $mailRecord) {
    $mailRecord->status = 0;
    $mailRecord->save();
  }
}else{
  $list = array();
  foreach ($res->user->getPotientialUserNetwork()->asArray() as $potentialUserNetwork) {
    $list[]="&quot; <img src='".$potentialUserNetwork->getPicture()."' width='16' height='16' /> ".$potentialUserNetwork->name()."&nbsp;"."(".$potentialUserNetwork->id.")"."&quot;";
  }
  if (count($list)>0) $res->memberList = implode(',', $list);
}
$res->useTemplate();
?>
