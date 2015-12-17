<?php
class Follower extends ActiveRecord\Model {
  static $validates_presence_of = array(
		array('follower_id'),
    array('followed_id'));

  /** accessible static function to know if someone follow another
   *
   * @param int $user_id
   * @param int $followed_id
   * @return int 
   */
  static function isFollow($user_id, $followed_id){
    return self::count(array('conditions' => array('follower_id = ? AND followed_id = ?', $user_id, $followed_id)));
  }
  
  /** accessible static function to know if someone is followed by another
   *
   * @param type $user_id
   * @param type $followed_id
   * @return type 
   */
  static function isFollowed($user_id, $followed_id){
    return self::count(array('conditions' => array('follower_id = ? AND followed_id = ?', $followed_id, $user_id)));
  }
  
  /** accesible function to know if 2 people know them eachothers
   *
   * @param type $user_id
   * @param type $followed_id
   * @return type 
   */
  static function isFriend($user_id, $followed_id){
    return self::count(array('conditions' => array('follower_id = ? AND followed_id = ?', $user_id, $followed_id))) == self::count(array('conditions' => array('followed_id = ? AND follower_id = ?', $user_id, $followed_id)));
  }

  /**
   * Create a notification after a new following
   */
  function after_create(){
  $membre=User::find($this->follower_id);
    Notification::create(
      array(
        'user_id' => $membre->id,
        'to_user_id' => $this->followed_id,
        'content' => $membre->name().' vous suit',
        'picture' => $membre->picture,
        'action' => 'timeline.php?id='.$membre->id,
        'status' => 0,
        'model' => 'Follower',
        'record' => $this->id
      )
    );
  }
}
?>
