<?php
include '../../inc/init.inc';
//$res->template = "back";
//$res->useTemplate();

foreach(User::all(array('conditions' => array('picture' => '')))->asArray() as $membre){
  $membre->picture = 'http://www.gravatar.com/avatar/'.md5($membre->email).'?d=mm';
  $membre->save();
  echo $membre->id;
}

echo 'finish';
?>
