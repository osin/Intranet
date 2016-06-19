<?php
class Tag extends ActiveRecord\Model
{
    static $validates_presence_of = array(
     array('name'));
    
   
  /** Get Questions using this tag
   *
   * @return collection
   */
  function getQuestions() {
    return Question::find_by_sql("SELECT * from questions
                                    WHERE id IN (SELECT question_id FROM questions_tags WHERE tag_id =" . $this->id. ")");
  }
}   
?>
