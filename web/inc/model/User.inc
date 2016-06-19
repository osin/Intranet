<?php

class User extends ActiveRecord\Model {

    static $PRIVACY_PUBLIC = 'public';
    static $PRIVACY_PRIVATE = 'private';

  static $validates_presence_of = array(
      array('password'));

  static $has_many = array(
      array('events'),
      array('activities'),
      );

  function asLink(){
    return Template::asLink('timeline', array('id' => $this->id));
  }
  
  function before_create(){
    if ($this->firstname && $this->lastname) {
      $this->name = $this->name();
    }
    if ($this->picture == "" || empty($this->picture)){
      $this->picture = 'http://www.gravatar.com/avatar/'.md5($this->email).'?d=mm';
    }
  }
  /** Return all activities of the user
   *
   * @return Collection Activities
   */
  function getActivities(){
    return Activity::find_by_sql("SELECT * from activities WHERE id IN (SELECT activity_id FROM activities_users WHERE user_id =" . $this->id . ")");
  }
  
  /** Return all activities of the user not finished
   *
   * @return Collection Activities
   */
  function getActivitiesNotFinished(){
    return Activity::find_by_sql("SELECT * from activities WHERE id IN (SELECT activity_id FROM activities_users WHERE user_id =" . $this->id . ") AND end >= '".date("Y-m-d H:i:s")."' ORDER BY end ASC");
  }
  
  /** Retrieve Events of an user, or for one user
   *
   * @param type $all
   * @return type 
   */
  function getEvents($all = false, $args=null){
     if ($all) {
        $ids = $this->getPotientialUserNetwork()->asID(true);
        $ids[] = $this->id;
        $args['conditions'] = array('user_id in(?)', $ids);
      }else{
        $args['conditions'] = array('user_id' => $this->id);
      }
      return Event::all($args);
  }

  /** Get People who follow the user
   *
   * @return Array 
   */
  function getFollowers() {
    return User::find_by_sql("SELECT * from users
                                    WHERE id IN (SELECT follower_id FROM followers WHERE followed_id =" . $this->id . ")");
  }
  
  /** Get only People the user follow
   *
   * @return Array 
   */
  function getFollowed() {
    return User::find_by_sql("SELECT * from users
                                    WHERE id IN (SELECT followed_id FROM followers WHERE follower_id =" . $this->id . ")");
  }  


  /** Get People friends, so followed and follower in the same time
   * @return Array
   */
  function getFriends() {
    return User::find_by_sql("
      SELECT * FROM users
        WHERE id IN (
          SELECT follower_id FROM followers
            WHERE followed_id =" . $this->id . ")
        AND id IN (
          SELECT followed_id FROM followers
            WHERE follower_id =" . $this->id . ")");
  }

  /** Get the most activity activity the user in.
   *
   * @return Object or NULL
   */
  function getLastActivity() {
    return Activity::find_by_sql("SELECT * from activities WHERE id IN (SELECT activity_id FROM activities_users WHERE user_id =" . $this->id . ") AND end >= '".date("Y-m-d H:i:s")."' ORDER BY end ASC")->first();
  }
  
  /** Return the picture profil of user coming from gravatar or himself
   *
   * @param type $size
   * @return type 
   */
  function getPicture($size = 300){
    if (stripos($this->picture, 'gravatar'))
      return $this->picture.'&size='.$size;
    return $this->picture;
  }
  
   /** Get Followed and Follower
   *
   * @return Array 
   */
  function getPotientialUserNetwork(){
    return User::find_by_sql("SELECT * from users
                                    WHERE id IN (SELECT followed_id FROM followers WHERE follower_id =" . $this->id . ") 
                                      OR id IN (SELECT follower_id FROM followers WHERE followed_id =" . $this->id . ")");
  }
  
  
  /** Know if user follow someone
   * @see Follower::isFollow()
   * @param type $followed_id
   * @return type 
   */
  function isFollow($followed_id){
    return Follower::isFollow($this->id, $followed_id);
  }
  
  /** Know if user is followed by someone
   * @see Follow::isFollowed()
   * @param type $user_id
   * @param type $followed_id
   * @return type 
   */
  function isFollowed($followed_id){
    return Follower::isFollowed($this->id, $followed_id);
  }
  
  /** Know if they are friends
   * @see Follow::isFriend()
   * @param type $user_id
   * @param type $followed_id
   * @return type 
   */
  function isFriend($followed_id){
    return Follower::isFriend($this->id, $followed_id);
  }
  
   /** return firstname & lastname
   * @see User::getName()
   * @return type 
   */
  function name(){
    if (isset($this->name) && $this->name != "")
      return $this->name;
    else
      return ucwords(strtolower($this->firstname.' '.$this->lastname));
  }
  
   /** call this function to follow the user
   * @param int $idToFollow
   * @return boolean
   */
  function setFollowUser($idToFollow) {
    if (User::exists($idToFollow) && $idToFollow != $this->id && !Follower::isFollow($this->id, $idToFollow)) {
      Follower::create(array(
                  'follower_id' => $this->id,
                  'followed_id' => $idToFollow
              ));
      return true;
    } else {
      return false;
    }
  }

  /** call this function to unfollow the user
   * @param int $idToUnFollow
   * @return boolean
   */
  function setUnfollowUser($idToUnfollow) {
    if (User::exists($idToUnfollow) && $idToUnfollow != $user->id && Follower::isFollow($this->id, $idToUnfollow)) {
      Follower::find('first', array('conditions' => array('follower_id = ? AND followed_id = ?', $this->id, $idToUnfollow)))->delete();
      return true;
    } else {
      return false;
    }
  }
  
}



?>
