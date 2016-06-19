<?php

include '../../inc/init.inc';
if ($id && Activity::exists($id)) {
  $activity = Activity::find($id);
}else
  $res->load('activities', array('error' => "4xx"));

$diff1 = $activity->start->diff($activity->end);
$now = new ActiveRecord\DateTime();
$diff2 = $now->diff($activity->end);
$is_end = $activity->end->getTimestamp() - $now->getTimestamp() > 0 ? false : true;

if ($diff2->days == 0 || $diff1->days == 0) {
  $res->diff = $is_end ? 100 : 99;
}else
  $res->diff = round((($diff1->days - $diff2->days) / $diff1->days * 100), 0, PHP_ROUND_HALF_DOWN);

$res->is_end = $is_end;

switch ($activity->status) {
  case 2:
    $res->status_bar = "success";
    $res->is_end = true;
    $res->diff = 100;
    break;
  case 1:
    $res->status_bar = "-striped progress-info";
    break;
  default:
    $res->status_bar = "info";
    break;
}

$res->home = Page::first(array('conditions' => array('activity_id' => $activity->id, 'title' => 'home')));
$res->pages = Page::all(array('conditions' => array('activity_id' => $activity->id)))->asArray();
$res->questions = "Les questions manquent là... :-)";
$res->comments = $activity->getComments()->asArray();
$res->organizers = $activity->getOrganizers()->asArray();
$res->activity = $activity;
$res->useTemplate("Liste des activités");
?>