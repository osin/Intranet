<?php

include '../../inc/init.inc';

if (isset($id) && is_numeric($id) && Question::exists($id)) {
  $res->question = Question::find($id);
  $res->responses =  Question::find('all', array('conditions' => array('question_id' => $id), 'order' => 'created_at asc'));
  $res->useTemplate("Liste des Questions");
} else {
    echo "La question n'existe pas";
}


