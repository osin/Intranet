<?php

class Questions_Tag extends ActiveRecord\Model {

    static $belongs_to = array(
      array(
          'question',
          'tag',
          ));
}

?>
