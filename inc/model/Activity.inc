<?php
class Activity extends ActiveRecord\Model
{
  static $status = array(0 => 'en préparation',
                         1 => 'ouvert',
                         2 => 'terminé');
  
  static $belongs_to = array(
      array('user'));
  
  static $validates_presence_of = array(
      array('name'),    
      array('status'),    
      array('user_id'),    
      array('start'),    
      array('end'),    
      array('name'));  
  
  
    
  
  /** Après qu'une activité soit crée, crée automatique la page home de celle ci.
   * 
   */
  function after_create(){
    Page::create(array(
            'title' => 'Home',
            'activity_id' => $this->id,
            'user_id' => $this->user_id,
            'content' => 'Le contenu est bien vide pour le moment. Il faudrait peut être le remplir non?'
            ));
  }
  
  /** return le lien vers la page de l'article
   * @return string
   */
  function asLink(){
      return Template::asLink('activity', array('id' => $this->id));
  }
  
  /** return les utilisateurs participants à une activité
   *
   * @return type 
   */
  function getOrganizers(){
    $activities = Activities_User::all(array('conditions' => array('activity_id' => $this->id)));
    $user_ids = $activities->asUser_id();
    $organizers = User::find($user_ids);
    if (!is_a($organizers, "Collection")) {
      $var = new Collection();
      $var->add($organizers);
      $organizers = $var;
    }
    return $organizers;
  }
  /** return le nom de l'activité
   *
   * @return string 
   */
  function name(){
      return ucfirst($this->name);
  }
    
}
?>
