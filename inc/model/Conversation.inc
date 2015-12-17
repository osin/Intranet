<?php

class Conversation extends ActiveRecord\Model {

  static $validates_presence_of = array(
      array('filename'));
  
  static $belongs_to = array(
      array('user')
  );

  function before_create() {
    $xml = new DOMDocument('1.0', 'utf-8');
    $tag = $xml->createElement('conversation');
    $xml->appendChild($tag);
    if (!is_dir(VAR_PATH . 'conversations')) {
      mkdir(VAR_PATH . 'conversations');
    }
    $xml->save(VAR_PATH . $this->filename);
  }

}

?>
