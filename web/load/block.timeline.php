<?php
$res->currentPage = isset($currentPage) ? $currentPage : 1;
$res->limit = $args['limit'] = isset($limit) ? $limit : 20;
$args['offset'] = ($res->currentPage - 1) * $args['limit'];
$args['order'] = isset($order) ? $order : 'updated_at desc';
if (isset($event) && is_numeric($event) && Event::find($event)->user_id == $res->membre->id) {
  $res->events = array(Event::find($event));
} else {
  $res->events = $res->membre->id == $res->user->id ? $res->membre->getEvents(true, $args) : $res->membre->getEvents(false, $args);
  $res->total = $res->membre->id == $res->user->id ? $res->membre->getEvents(true)->count : $res->membre->getEvents(false)->count;
}
$res->useTemplate();
?>
