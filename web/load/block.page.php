<?php

$res->edit = true;
if (isset($id) && Page::exists($id)) {
  $page = Page::find($id);
  $res->title = $page->title;
  $res->content = $page->content;
  $res->id = $page->id;
  
  if (isset($page->activity_id) && $page->activity_id > 0)
    $res->edit = $res->user->id == $page->user_id;
  else
    $res->edit = 2;
}
else{
  $res->title = "";
  $res->content = "";
  $res->id = "";
}
if (isset($activity) && is_numeric($activity) && $res->user->id == Activity::find($activity)->id) {
  $res->activity = $activity;
}
$res->useTemplate();
?>
