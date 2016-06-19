<?php

class Comment extends ActiveRecord\Model {

  static $validates_presence_of = array(
      array('user_id'),
      array('model'),
      array('record'),
      array('content'));
  static $belongs_to = array(
      array('user')
  );
  
  /** We directly retrieve the object used by the comment record find by model><record
   *
   * @return ActiveRecord Object or Null
   */
  function getObject() {
    if (!isset($this->model) && !isset($this->record))
      return;
    return call_user_func_array(array('\\' . $this->model, 'find'), array($this->record));
  }
  
  /**
   * Create a notification after a new comment
   */
  function after_create(){
  $referer_id = $this->getObject()->user_id;
  $membre = User::find($this->user_id);
  if ($this->user_id == $referer_id) {
    return;
  }
    $notif = Notification::create(
      array(
        'user_id' => $membre->id,
        'to_user_id' => $referer_id,
        'content' => $membre->name().' a commenté votre publication',
        'picture' => $membre->picture,
        'action' => $this->getObject()->asLink(),
        'status' => 0,
        'model' => $this->model,
        'record' => $this->record,
      )
    );
  }
}

?>
