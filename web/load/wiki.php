<?php
include '../../inc/init.inc';
isset($conditions) ? $args['conditions'] = $conditions : '';
$res->total = isset($args) ? Page::count($args) : Page::count();
$res->currentPage = isset($currentPage) ? $currentPage : 1;
$res->limit = $args['limit'] = isset($limit) ? $limit : 7;
$args['offset'] = ($res->currentPage-1)*$args['limit'];
$args['order'] = isset($order) ? $order : 'created_at desc';
$res->pages = Page::all($args);
$res->search_opts = array(0 => array('label' => 'Quel article cherchez vous ?', 'field' => 'title', 'type' => 'text', 'class' => 'xxlarge'));
$res->useTemplate("Wiki");
?>