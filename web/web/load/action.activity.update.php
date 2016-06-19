<?php

include '../../inc/init.inc';

if (isset($id) && is_numeric($id) && Activity::exists($id)) {
  $activity = Activity::find($id);
  if ($activity->user_id != $res->user->id) {
    throw new Exception('Unauthorized action');
  }
} else {
  $activity = new Activity();
}
$activity->name = $name;
$activity->start = $start. " ".$startTime;
$activity->end = $end." ".$endTime;
$activity->status = $status;
$activity->privacy = $privacy;
$activity->description = $description;
$activity->user_id = $res->user->id;

if ($activity->is_valid()) {
  $activity->save();
  if (isset($member_list_values)) {
  preg_match_all('#\(([^)]+)\)#', $member_list_values, $organizers_ids);
  Activities_User::table()->delete(array('activity_id' => $activity->id));
  $organizers_ids[1][] = $activity->user_id;
  foreach ($organizers_ids[1] as $organizers_id) {
    if (!User::exists($organizers_id)) {
      continue;
    }
    $activity_user = Activities_User::create(array('activity_id' => $activity->id, 'user_id' => $organizers_id));
  }
}
$res->load('activity', array('id' => $activity->id));
}else{
 $res->load('activity.create.base', array('id' => $activity->id, 'infos' => array('error' => 'Certaines informations manquent'))); 
}
?>
