<?php
include '../../inc/init.inc';
if (isset($model) && isset($record) && isset($content)) {
  $comment = Comment::create(array(
      'model' => $model,
      'record' => $record,
      'content' => $content,
      'user_id' => $res->user->id,
      ));
}
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>
