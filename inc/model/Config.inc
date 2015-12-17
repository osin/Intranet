<?php
class Config extends ActiveRecord\Model{
  static $validates_presence_of = array(
      array('name'),
      array('value'),
      );

}
?>
