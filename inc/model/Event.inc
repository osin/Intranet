<?php

class Event extends ActiveRecord\Model {
  
  static $belongs_to = array(
     array('user')
  );

  /** Put here the name of the class you want to show for Events Timeline and other shareposting infos. 
   */
  static $unAllowClass = array(
      'Question', 'Questions_Tag', 'Tag',
      'User', 'Follower' => array('type' => array('delete')), 'Comment', array('type' => array('delete')),
      'Notification', 'Mail', 'Conversation', 
      'Activities_User', );
  
  static $validates_presence_of = array(
      array('user_id'));
  
  static $verbs = array(
      //verb => languages => singular form of past time, plural form of past time
      'connecter' => array('fr' => array('est connecté'), 'en' => array('connected')),
      'créer' => array('fr' => array('a crée'), 'en' => array('created')),
      'inscrire' => array('fr' => array('est inscrit'), 'en' => array('suscribed')),
      'modifier' => array('fr' => array('a édité'), 'en' => array('edit')),
      'mise à jour' => array('fr' => array('mise à jour'), 'en' => array('updated')),
      'rejoindre' => array('fr' => array('a rejoint'), 'en' => array('joined')),
      'rompre' => array('fr' => array('a rompu'), 'en' => array('broke')),
      'suivre' => array('fr' => array('suit'), 'en' => array('follow')),
      'supprimer' => array('fr' => array('a supprimé'), 'en' => array('deleted')),
      'terminer' => array('fr' => array('a terminé'), 'en' => array('finished')),
  );
  static $models = array(
      //models, action_type => 'verbs'
      'Activity' => array(
          'create' => 'créer',
          'update' => 'mise à jour',
      ),
      'Activities_User' => array(
          'create' => 'rejoindre',
          'update' => 'mise à jour',
      ),
      'Follower' => array(
          'create' => 'suivre',
      ),
      'Page' => array(
          'create' => 'créer',
          'update' => 'mise à jour',
          'delete' => 'supprimer',
      ),
      
  );
  
  function asLink(){
    return Template::asLink('timeline', array('id' => $this->user_id, 'block' => 'timeline', 'event' => $this->record));
  }

  /** Generate the content of an Event
   *
   * @return string content 
   */
  function createContent() {
    if (!isset($this->model) || !isset($this->record) || !isset($this->log_id))
      return 'invalide log_id, model or record';
    $log = Log::find($this->log_id);
    $user = User::find($this->user_id);
    $args['name'] = '<a href="' . $user->asLink() . '" title="' . $user->name() . '">' . $user->name() . '</a>';
    $args['verb'] = self::$verbs[self::$models[$this->model][$log->type]]['fr'][0];
    $args['complement'] = $this->getComplement();
    return implode(' ', $args);
  }

  /*   * get the complement of the content
   * 
   * @return string 
   */

  function getComplement() {
    switch ($this->model) {
      case 'Follower':
        $object = Log::find($this->log_id)->getObject();
        $user = User::find($object->followed_id);
        return '<a href="' . $user->asLink() . '" title="' . $user->name() . '">' . $user->name() . '</a>';
      case 'Activity':
        $object = Log::find($this->log_id)->getObject();
        return '<a href="' . $object->asLink() . '" title="' . $object->name() . '">' . $object->name() . '</a>';
      case 'Page':
        $object = Log::find($this->log_id)->getObject();
        return '<a href="' . $object->asLink() . '" title="' . $object->title() . '">' . $object->title() . '</a>';
      default:
        $object = Log::find($this->log_id)->getObject();
        if (isset($object->name) && $object->name!= "") {
          return $object->name;
        }elseif(isset($object->title) && $object->title!= "") {
          return $object->title;
        }
        return;
    }
  }

  /** prepare the content of an event and return it if available
   *
   * @param type $prepare
   * @return string 
   */
  function getContent($prepare = true) {
    if (!isset($this->data) || $this->data == NULL) {
      if ($this->content == "" && isset($this->log_id)) {
        return $this->createContent();
      }else
        return '<div class="padding30">
      <p>'.$this->content . '</p>
      <br clear="all" />
      </div>';
    }
    $data = json_decode($this->data);
    if ($data == null) {
        return 'Problem to parse content';
    }
    if (isset($data->title) && $data->title != "") {
      $title = $data->title;
    } else {
      $title = "";
    }
    if (isset($this->link) && $this->link != "") {
      //need to be closed by </a>
      $link1 = '<a href = "' . $this->link . '" title="">';
      $link2 = '</a>';
    }else
      $link1 = $link2 = "";
    $str = '<div class="event_bar">';
    if (isset($data->image) && (count(explode('youtube', $this->link))<=1)) {
        $style = $data->image->width > 400 ? 'float:initial;':'';
        $class= $data->image->width > 400 ? '':'padding30';
      $str .= '<div class="event_image '.$class.'" style=" display: block; margin: 0 auto;'.$style.'">'
              . $link1 . '<img src="' . $data->image->img . '" width="100%" height="100%" alt="' . $title . '_image" />' . $link2
              . '</div>';
    }
    
    if (isset($data->description) && $data->description != "") {
      $str.= '<div class="event_info span6">
      <label class="event_desc">' . $data->description . '</label>
      <br clear="all" />
      </div>';
    }
    

    if ($link1 != $link2){
      if (count(explode('youtube', $this->link))>1) {
        $url_info = parse_url($this->link);
        $url_args = explode('&', ($url_info['query']));
        $url_info = explode('=', ($url_args[0]));
        $str.= '<div class="video-container"><iframe src="http://www.youtube.com/embed/'.$url_info[1].'" frameborder="0" allowfullscreen></iframe></div>';
      }  
    }
    $str .= '<br clear="all" /></div>';
    return $str;
  }

  /** return the list of event associated to the user
   * @see User::getEvents()
   * @param type $user_id
   * @param type $all
   * @return type 
   */
  static function getEvents($user_id, $all = false, $args=null) {
    return User::find($user)->getEvents($all, $args);
  }

  /** Call the web service to parse the content of an event
   *
   * @return json Data 
   */
  static function parser() {
    return file_get_contents(SITE_PATH."/service/parse.link.php");
  }

}

?>
