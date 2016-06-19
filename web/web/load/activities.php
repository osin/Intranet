<?php
include '../../inc/init.inc';
isset($conditions) ? $args['conditions'] = $conditions : '';
$res->total = isset($args) ? Activity::count($args) : Activity::count();
$res->currentPage = isset($currentPage) ? $currentPage : 1;
$res->limit = $args['limit'] = isset($limit) ? $limit : 7;
$args['offset'] = ($res->currentPage - 1) * $args['limit'];
$args['order'] = isset($order) ? $order : 'created_at desc';
$res->activities = Activity::all($args);
if (isset($error) && $error = "4xx")
  $res->useTemplate("Liste des activités", array('error' => "l'activité n'existe pas"));
else
  $res->useTemplate("Liste des activités");