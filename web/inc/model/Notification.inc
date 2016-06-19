<?php

class Notification extends ActiveRecord\Model {

  static $validates_presence_of = array(
      array('content'),
      array('to_user_id'));

  static $belongs_to = array(
      array('user')
  );

}

?>
