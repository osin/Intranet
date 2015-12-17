<?php

class Mail extends ActiveRecord\Model {
  static $validates_presence_of = array(
		array('message'),
    array('referer_id'),
    array('user_id'));
  
  static $belongs_to = array(
     array('expeditor', 'class_name' => 'User', 'foreign_key' => 'user_id'),
     array('user'),
     array('recipient', 'class_name' => 'User', 'foreign_key' => 'referer_id'),
  );
  
}
?>