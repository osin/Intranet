<?php

include '../../inc/init.inc';

if (isset($tag)){
  $conditions = array('id in (?)', Questions_Tag::find('all', array('conditions' => array('tag_id' => Tag::find_by_name($tag)->id)))->asQuestion_ID());
}
isset($conditions) ? $args['conditions'] = $conditions : $args['conditions'] = 'title NOT LIKE ""';
$res->total = isset($args) ? Question::count($args) : Question::count();
$res->currentPage = isset($currentPage) ? $currentPage : 1;
$res->limit = $args['limit'] = isset($limit) ? $limit : 7;
$args['offset'] = ($res->currentPage - 1) * $args['limit'];
$args['order'] = isset($order) ? $order : 'created_at desc';
$res->questions = Question::all($args);

$res->tags = Tag::find('all', array('order' => 'Rand()', 'limit' => 20));
if (!isset($infos))
  $infos = array();
$res->search_opts = array(0 => array('label' => 'Quelle question vous posez vous ?', 'field' => 'title', 'type' => 'text', 'class' => 'xxlarge'));
$res->useTemplate("Questions - Reponses", $infos);
?>