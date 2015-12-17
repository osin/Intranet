<?php
include '../../inc/init.inc';
$list = array();
  foreach ($res->user->getPotientialUserNetwork()->asArray() as $potentialUserNetwork) {
    $list[]="&quot; <img src='".$potentialUserNetwork->getPicture(16)." /> ".$potentialUserNetwork->name()."&nbsp;"."(".$potentialUserNetwork->id.")"."&quot;";
  }
  if (count($list)>0) $res->memberList = implode(',', $list);
if(isset($id)){
  $res->activity = Activity::find($id);
  $res->is_new = false;
}else{
  $res->activity = new Activity();
  $res->is_new = true;
}
if (isset($infos))
  $res->useTemplate('Administration de l\'activité', $infos);
else
  $res->useTemplate('Administration de l\'activité');
?>
