<?php

include '../../inc/init.inc';
if (!is_string($title) || !is_string($content)) {
  throw new Exception('data is empty');
}
if (is_numeric($id)) {
  $page = Page::find($id);
}else
  $page = new Page();

$page->title = $title;
$page->content = $content;
$page->user_id = $res->user->id;

if (isset($activity) && is_numeric($activity) && $res->user->id == Activity::find($activity)->id) {
  $page->activity_id = $activity;
}
$page->save();
$res->load('page', array('id' => $page->id));
?>