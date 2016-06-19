<?php

include '../../inc/init.inc';
if (isset($id) && Page::exists($id)) {
  $page = Page::find($id);
} else {
  $page = new Page();
}
if (isset($activity) && is_numeric($activity) && $res->user->id == Activity::find($activity)->id) {
  $res->activity_id = $activity;
}  else {
  $res->activity_id = '';
}
$res->title = $page->title;
$res->content = $page->content;
$res->page = $page;
$res->id = $page->id;
$res->useTemplate();
?>
