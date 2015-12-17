<?php

class Activities_User extends ActiveRecord\Model
{
    static $belongs_to = array(
      array('users'),
      array('activities')
        );
}
?>
