<?php
class Connector extends ActiveRecord\Model
{
    static $validates_presence_of = array(
      array('api_key'), array('api_secret'), array('name'));
}
?>