<?php

include '../../inc/init.inc';
isset($conditions) ? $args['conditions'] = $conditions : '';
$res->total = isset($args) ? User::count($args) : User::count();
$res->currentPage = isset($currentPage) ? $currentPage : 1;
$res->limit = $args['limit'] = isset($limit) ? $limit : 8;
$args['offset'] = ($res->currentPage - 1) * $args['limit'];
$args['order'] = isset($order) ? $order : 'created_at desc';
$res->search_opts = array(
    0 => array('label' => 'Rechercher un membre', 'field' => 'name', 'type' => 'text', 'class' => 'medium'),
);
$res->membres = User::all($args);

$res->useTemplate('Liste des membres', isset($new_member) ? array("success" => "Bienvenue " . $res->user->name . ", voici les autres membres inscrits...") : "");