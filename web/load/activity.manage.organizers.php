<?php
include '../../inc/init.inc';
if (!$id || !is_numeric($id))
  throw new Exception("No activity given");

if (!Activity::exists($id))
  throw new Exception('Not Found');
$activity = Activity::find($id);

if ($activity->user_id != $res->user->id)
  throw new Exception('Not authorized');

$res->organizers = $activity->getOrganizers();
$list = array();
  foreach ($res->user->getPotientialUserNetwork()->asArray() as $potentialUserNetwork) {
    $list[]="&quot; <img src='".$potentialUserNetwork->getPicture(16)." /> ".$potentialUserNetwork->name()."&nbsp;"."(".$potentialUserNetwork->id.")"."&quot;";
  }

  $res->memberListValues = implode('-', $activity->getOrganizers()->asID());

  foreach ($res->organizers->asArray() as $memberListUnit) {
    $res->memberList .= "<p>".$memberListUnit->name()." "."(".$memberListUnit->id.")"."</p>";
}
$res->useTemplate("Membre de votre ".$activity->name());
?>
