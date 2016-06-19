<?php

class Question extends ActiveRecord\Model {

  static $validates_presence_of = array(
      array('user_id'),
      array('content'));
  
  static $belongs_to = array(
      array(
          'user',
          'question',
          ));


  /**
   * Return the url of the corresponding questions
   * @return type 
   */
  function asLink() {
    return Template::asLink('question', array('id' => $this->id));
  }

  /** Get Tags of the questions
   *
   * @return collection
   */
  function getTags() {
    return Tag::find_by_sql("SELECT * from tags
                                    WHERE id IN (SELECT tag_id FROM questions_tags WHERE question_id =" . $this->id . ")");
  }

}

?>
