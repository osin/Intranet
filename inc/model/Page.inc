<?php
class Page extends ActiveRecord\Model
{
    static $validates_presence_of = array(
      array('user_id'),
      array('title'),
      array('content'),
        );
    
    static $belongs_to = array(
      array('user'),
      array('activity'));
     
  /** return le lien vers la page de l'article
   * @return string
   */
  function asLink(){
      return Template::asLink('page', array('id' => $this->id));
  }
  
  /** Return the name of the page
   * 
   * @return string
   */
  function title(){
      return ucwords($this->title);
  }
}
?>
